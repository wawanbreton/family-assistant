#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QQueue>
#include <QSet>
#include <QUrl>

#include "hardware/music/song.h"

class DlnaPlaylist : public QObject
{
    Q_OBJECT

public:
    explicit DlnaPlaylist(const QUrl& device_description_url, QObject* parent = nullptr);

    Song playNext();

    Song playPrevious();

    bool hasPrevious() const
    {
        return current_song_ > 0;
    }

    void clearHistory()
    {
        planned_songs_.clear();
        current_song_ = -1;
        emit hasPreviousChanged();
    }

signals:
    void hasPreviousChanged();

private:
    struct Container
    {
        QString name;
        QString id;
        int depth;
    };

    struct SongEntry
    {
        Song song;
        quint32 score;
    };

    void updateSongs();

    void getDeviceDescription();

    void onGetDeviceDescriptionFinished(QNetworkReply* reply);

    void browseNextContainer();

    void browseContainer(const Container& container);

    void onGetBrowseContainerFinished(QNetworkReply* reply, const Container& container);

    void mergeSongs(const QMap<QString, Song>& new_songs);

private:
    static constexpr int songs_before_current = 10;

private:
    QNetworkAccessManager* const network_manager_{ nullptr };
    const QUrl device_description_url_;
    QUrl content_directory_url_;
    QQueue<Container> containers_to_browse_;
    const QStringList root_container_path_;
    QMap<QString, Song> discovered_songs_;
    QQueue<Container> root_songs_containers_;
    QMap<QString, SongEntry> songs_;
    QQueue<QString> planned_songs_;
    int current_song_{ -1 };
};
