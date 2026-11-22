#include "dlnaplaylist.h"

#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStack>
#include <QTimer>
#include <QtXml/QDomDocument>
#include <algorithm>
#include <chrono>
#include <numeric>

#include "data/preferences.h"

#define DISPLAY_DEBUG 0

using namespace std::chrono_literals;


DlnaPlaylist::DlnaPlaylist(const QUrl& device_description_url, QObject* parent)
    : QObject(parent)
    , network_manager_(new QNetworkAccessManager(this))
    , device_description_url_(device_description_url)
    , root_container_path_(Preferences::get()->getString(PreferenceEntry::DlnaMainMusicPath).trimmed().split('/'))
{
    getDeviceDescription();
}

Song DlnaPlaylist::playNext()
{
    if (songs_.isEmpty())
    {
        return Song();
    }

    ++current_song_;
    while (current_song_ >= planned_songs_.count())
    {
        const quint32 total_score = std::accumulate(
            songs_.begin(),
            songs_.end(),
            quint32{ 0 },
            [](const quint32 total, const SongEntry& song) { return total + song.score; });

        const quint32 winner = std::rand() % total_score;
        quint32 current_score = 0;
        bool winner_found = false;
        QString song_id;

        for (auto iterator = songs_.begin(); iterator != songs_.end(); ++iterator)
        {
            if (winner_found)
            {
                ++iterator.value().score;
            }
            else
            {
                current_score += iterator.value().score;
                if (current_score > winner)
                {
                    song_id = iterator.key();
                    winner_found = true;
                    iterator.value().score = 0;
                }
                else
                {
                    ++iterator.value().score;
                }
            }
        }
        planned_songs_.append(song_id);
    }

    while (planned_songs_.count() > songs_before_current)
    {
        planned_songs_.dequeue();
        --current_song_;
    }

    emit hasPreviousChanged();
    return songs_[planned_songs_.value(current_song_)].song;
}

Song DlnaPlaylist::playPrevious()
{
    if (planned_songs_.count() > 1)
    {
        current_song_ = std::max(0, current_song_ - 1);
        emit hasPreviousChanged();
        return songs_.value(planned_songs_.value(current_song_)).song;
    }

    return Song();
}

void DlnaPlaylist::updateSongs()
{
    discovered_songs_.clear();
    containers_to_browse_ = root_songs_containers_;
    browseNextContainer();
}

void DlnaPlaylist::getDeviceDescription()
{
    QNetworkReply* reply = network_manager_->get(QNetworkRequest(device_description_url_));
    connect(reply, &QNetworkReply::finished, this, [this, reply] { onGetDeviceDescriptionFinished(reply); });
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}

void DlnaPlaylist::onGetDeviceDescriptionFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Error when to reading DLNA device description:" << reply->errorString();
        return;
    }

    QDomDocument xml;
    QDomDocument::ParseResult parse_success = xml.setContent(reply);
    if (! parse_success)
    {
        qWarning() << "DLNA device description is not a valid XML document" << parse_success.errorMessage;
        return;
    }

    { // Parse server name, for display purposes only
        const QDomElement friendly_name_element = xml.firstChildElement("friendlyName");
        if (! friendly_name_element.isNull())
        {
            qInfo() << "Connected to DLNA server" << friendly_name_element.text();
        }
    }

    QString control_url;
    const QDomNodeList service_elements = xml.elementsByTagName("service");
    for (size_t i = 0; i < service_elements.size(); ++i)
    {
        const QDomElement service_element = service_elements.at(i).toElement();
        QString service_type;
        QString service_control_url;

        const QDomNodeList service_children = service_element.childNodes();
        for (size_t j = 0; j < service_children.size(); ++j)
        {
            const QDomNode service_child = service_children.at(j);
            if (service_child.isElement())
            {
                const QDomElement service_child_element = service_child.toElement();
                if (service_child_element.nodeName() == "serviceType")
                {
                    service_type = service_child_element.text();
                }
                else if (service_child_element.nodeName() == "controlURL")
                {
                    service_control_url = service_child_element.text();
                }
            }
        }

        if (service_type.startsWith(QStringLiteral("urn:schemas-upnp-org:service:ContentDirectory:")))
        {
            control_url = service_control_url;
        }
    }

    if (control_url.isEmpty())
    {
        qWarning() << "DLNA device description has no ContentDirectory service";
        return;
    }

    content_directory_url_ = device_description_url_.resolved(QUrl(control_url));
    containers_to_browse_.enqueue({ .name = QStringLiteral("<root>"), .id = QStringLiteral("0"), .depth = -1 });
    browseNextContainer();

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DlnaPlaylist::updateSongs);
    timer->start(1min);
}

void DlnaPlaylist::browseNextContainer()
{
    if (containers_to_browse_.isEmpty())
    {
        mergeSongs(discovered_songs_);
        discovered_songs_.clear();
    }
    else
    {
        browseContainer(containers_to_browse_.dequeue());
    }
}

void DlnaPlaylist::browseContainer(const Container& container)
{
#if DISPLAY_DEBUG
    qDebug() << "Browse container" << container.name << container.id << container.depth;
#endif

    QDomDocument object_id_doc;
    QDomElement object_id_element = object_id_doc.createElement("ObjectID");
    object_id_element.appendChild(object_id_doc.createTextNode(container.id));
    object_id_doc.appendChild(object_id_element);

    QByteArray body = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                      "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                      "<s:Body><u:Browse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\">";
    body += object_id_doc.toByteArray(0).replace("\n", "");
    body += "<BrowseFlag>BrowseDirectChildren</BrowseFlag>"
            "<Filter>*</Filter><StartingIndex>0</StartingIndex><RequestedCount>0</RequestedCount>"
            "<SortCriteria></SortCriteria></u:Browse></s:Body></s:Envelope>";

    QNetworkRequest request(content_directory_url_);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("text/xml; charset=\"utf-8\""));
    request.setRawHeader("SOAPAction", "\"urn:schemas-upnp-org:service:ContentDirectory:1#Browse\"");

    QNetworkReply* reply = network_manager_->post(request, body);
    connect(
        reply,
        &QNetworkReply::finished,
        this,
        [this, reply, container] { onGetBrowseContainerFinished(reply, container); });
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}

void DlnaPlaylist::onGetBrowseContainerFinished(QNetworkReply* reply, const Container& container)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Error when browsing DLNA object:" << reply->errorString();
        browseNextContainer();
        return;
    }

    QDomDocument xml;
    QDomDocument::ParseResult parse_success = xml.setContent(reply);
    if (! parse_success)
    {
        qWarning() << "DLNA browse object description is not a valid XML document" << parse_success.errorMessage;
        browseNextContainer();
        return;
    }

    QQueue<Container> discovered_containers;
    const int sub_containers_depth = container.depth + 1;
    const std::optional<QString> root_container_folder_name
        = sub_containers_depth < root_container_path_.size()
            ? std::make_optional(root_container_path_.at(sub_containers_depth))
            : std::nullopt;

    const QDomElement root_element = xml.firstChild().toElement();
    const QString xml_schema = root_element.attribute("xmlns:s");
    if (xml_schema == QStringLiteral("http://schemas.xmlsoap.org/soap/envelope/"))
    {
        const QDomNodeList result_elements = root_element.elementsByTagName("Result");
        for (size_t i = 0; i < result_elements.size(); ++i)
        {
            QDomDocument xml_didl;
            QDomDocument::ParseResult didl_result = xml_didl.setContent(result_elements.at(i).toElement().text());
            if (! didl_result)
            {
                qWarning() << "DIDL object description is not a valid XML document" << didl_result.errorMessage;
                continue;
            }

            {
                const QDomNodeList container_elements = xml_didl.elementsByTagName("container");
                for (size_t j = 0; j < container_elements.size(); ++j)
                {
                    const QDomElement container_element = container_elements.at(j).toElement();
                    const QString id = container_element.attribute("id");
                    const QString title = container_element.firstChildElement("dc:title").text();
                    const QString parent_id = container_element.attribute("parentID");

                    if (id.isEmpty() || title.isEmpty() || parent_id.isEmpty())
                    {
                        qWarning() << "Incomplete DLNA container" << id << title << parent_id;
                        continue;
                    }

                    if (root_container_folder_name.has_value() && title != *root_container_folder_name)
                    {
                        continue;
                    }

#if DISPLAY_DEBUG
                    qDebug() << "Enqueue listing of" << id << title;
#endif
                    discovered_containers.enqueue({ .name = title, .id = id, .depth = sub_containers_depth });
                }
            }

            {
                const QDomNodeList item_elements = xml_didl.elementsByTagName("item");
                for (size_t j = 0; j < item_elements.size(); ++j)
                {
                    const QDomElement item_element = item_elements.at(j).toElement();

                    Song song;
                    song.path = item_element.firstChildElement("res").text();
                    song.icon_path = item_element.firstChildElement("upnp:albumArtURI").text();
                    song.artist = item_element.firstChildElement("upnp:artist").text();
                    song.title = item_element.firstChildElement("dc:title").text();

                    const QString id = item_element.attribute("id");

                    if (id.isEmpty() || song.path.isEmpty() || song.title.isEmpty())
                    {
                        qWarning() << "Incomplete song data" << item_element.text();
                        continue;
                    }

                    discovered_songs_.insert(id, std::move(song));
                }
            }
        }
    }

    if (sub_containers_depth == root_container_path_.size() - 1)
    {
        root_songs_containers_ = discovered_containers;
    }

    containers_to_browse_.append(discovered_containers);
    browseNextContainer();
}

void DlnaPlaylist::mergeSongs(const QMap<QString, Song>& new_songs)
{
#if DISPLAY_DEBUG
    qDebug() << "merge list of" << new_songs.size() << "songs";
#endif

    {
        QMutableMapIterator<QString, SongEntry> iterator(songs_);
        while (iterator.hasNext())
        {
            iterator.next();

            auto new_desc_iterator = new_songs.find(iterator.key());
            if (new_desc_iterator != new_songs.end())
            {
                // Update existing song description
                iterator.value().song = new_desc_iterator.value();
            }
            else
            {
                // Song doesn't exist anymore, remove
                iterator.remove();
            }
        }
    }

    {
        // List all songs to add
        QList<QMap<QString, Song>::const_iterator> songs_to_add;
        for (auto iterator = new_songs.begin(); iterator != new_songs.end(); ++iterator)
        {
            if (! songs_.contains(iterator.key()))
            {
                songs_to_add << iterator;
            }
        }

        quint32 new_songs_score = songs_.size() + songs_to_add.size();
        for (QMap<QString, Song>::const_iterator song_to_add_iterator : songs_to_add)
        {
            songs_.insert(
                song_to_add_iterator.key(),
                SongEntry{ .song = song_to_add_iterator.value(), .score = new_songs_score });
        }
    }
}
