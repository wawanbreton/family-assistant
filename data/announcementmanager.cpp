#include "announcementmanager.h"

#include <QTimer>
#include <easyqt/datastorage.h>
#include <piper.h>

#include "data/announcementeventplay.h"
#include "data/announcementeventwait.h"


SINGLETON_IMPL(AnnouncementManager)

AnnouncementManager::AnnouncementManager(QObject* parent)
    : QObject{ parent }
{
    const QString voice = "fr_FR-siwis-medium";
    const QString voice_filepath
        = easyqt::DataStorage::resourceFile(QString("%1.onnx").arg(voice), easyqt::ResourceType::Voice);
    const QString voice_desc_filepath
        = easyqt::DataStorage::resourceFile(QString("%1.onnx.json").arg(voice), easyqt::ResourceType::Voice);
    const QString espeak_data_path = easyqt::DataStorage::resourceFile("espeak-ng-data", easyqt::ResourceType::Voice);
    voice_synthesizer_ = piper_create(
        voice_filepath.toStdString().c_str(),
        voice_desc_filepath.toStdString().c_str(),
        espeak_data_path.toStdString().c_str());
}

AnnouncementManager::~AnnouncementManager()
{
    SINGLETON_DESTROY_IMPL(AnnouncementManager);
    piper_free(voice_synthesizer_);
}

void AnnouncementManager::addAnnouncement(const QString& text, const quint8 repeat, const std::chrono::seconds delay)
{
    if (repeat == 0)
    {
        return;
    }

    const bool start_next_event = events_queue_.empty();

    for (quint8 index = 0; index < repeat; ++index)
    {
        if (index > 0)
        {
            events_queue_ << new AnnouncementEventWait(delay, this);
        }
        events_queue_ << new AnnouncementEventPlay(text, this);
    }

    if (start_next_event)
    {
        startNextEvent();
    }
}

piper_synthesizer* AnnouncementManager::getVoiceSynthesizer()
{
    return voice_synthesizer_;
}

void AnnouncementManager::onEventOver()
{
    events_queue_.dequeue()->deleteLater();
    startNextEvent();
}

void AnnouncementManager::startNextEvent()
{
    if (! events_queue_.empty())
    {
        AbstractAnnouncementEvent* event = events_queue_.head();
        connect(event, &AbstractAnnouncementEvent::over, this, &AnnouncementManager::onEventOver);
        event->start();
    }
}
