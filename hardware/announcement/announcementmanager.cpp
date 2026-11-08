#include "announcementmanager.h"

#include <QAudioDevice>
#include <QMediaDevices>
#include <QTimer>
#include <easyqt/datastorage.h>
#include <piper.h>

#include "data/preferences.h"
#include "hardware/announcement/announcementeventplay.h"
#include "hardware/announcement/announcementeventwait.h"


SINGLETON_IMPL_WITH_1_PARAM(AnnouncementManager, const bool skip_voice_engine, skip_voice_engine)

AnnouncementManager::AnnouncementManager(QObject* parent, const bool skip_voice_engine)
    : QObject{ parent }
    , output_device_(QMediaDevices::defaultAudioOutput())
{
    Preferences::access()->registerPreference(PreferenceEntry::AnnouncementRepeats, QMetaType::Int, 3);
    Preferences::access()->registerPreference(PreferenceEntry::AnnouncementFirstVolume, QMetaType::Double, 1.0);
    Preferences::access()->registerPreference(PreferenceEntry::AnnouncementLastVolume, QMetaType::Double, 1.0);
    Preferences::access()->registerPreference(PreferenceEntry::AnnouncementAudioDevice, QMetaType::QString, "");
    repeat_ = Preferences::get()->getInt(PreferenceEntry::AnnouncementRepeats);

    const QString announcement_audio_device_name
        = Preferences::get()->getString(PreferenceEntry::AnnouncementAudioDevice).trimmed();
    if (! announcement_audio_device_name.isEmpty())
    {
        bool output_device_found = false;
        for (const QAudioDevice& device : QMediaDevices::audioOutputs())
        {
            if (device.description().trimmed() == announcement_audio_device_name)
            {
                output_device_ = device;
                output_device_found = true;
            }
        }

        if (output_device_found)
        {
            qInfo() << "Playing announcements on" << output_device_.description();
        }
        else
        {
            qWarning() << "Output device" << announcement_audio_device_name << "was not found, using default"
                       << output_device_.description();
        }
    }


    if (skip_voice_engine)
    {
        qWarning() << "Announcements are disabled";
    }
    else
    {
        QTimer::singleShot(0, this, [this] { addAnnouncement("Bienvenue dans l'assistant familial !", 1); });

        const QString voice = "fr_FR-siwis-medium";
        const QString voice_filepath
            = easyqt::DataStorage::resourceFile(QString("%1.onnx").arg(voice), easyqt::ResourceType::Voice);
        const QString voice_desc_filepath
            = easyqt::DataStorage::resourceFile(QString("%1.onnx.json").arg(voice), easyqt::ResourceType::Voice);
        const QString espeak_data_path
            = easyqt::DataStorage::resourceFile("espeak-ng-data", easyqt::ResourceType::Voice);
        voice_synthesizer_ = piper_create(
            voice_filepath.toStdString().c_str(),
            voice_desc_filepath.toStdString().c_str(),
            espeak_data_path.toStdString().c_str());
    }
}

AnnouncementManager::~AnnouncementManager()
{
    SINGLETON_DESTROY_IMPL(AnnouncementManager);
    if (voice_synthesizer_)
    {
        piper_free(voice_synthesizer_);
    }
}

void AnnouncementManager::addAnnouncement(
    const QString& text,
    const std::optional<quint8> repeat,
    const std::chrono::seconds delay)
{
    const quint8 actual_repeat = repeat.value_or(repeat_);
    if (actual_repeat == 0)
    {
        return;
    }

    const bool start_next_event = events_queue_.empty();

    const double first_volume = Preferences::get()->getDouble(PreferenceEntry::AnnouncementFirstVolume);
    const double last_volume = Preferences::get()->getDouble(PreferenceEntry::AnnouncementLastVolume);

    for (quint8 index = 0; index < actual_repeat; ++index)
    {
        if (index > 0)
        {
            events_queue_ << new AnnouncementEventWait(delay, this);
        }

        const double announcement_factor
            = actual_repeat == 1 ? 0.5 : (static_cast<double>(index) / (actual_repeat - 1));
        events_queue_ << new AnnouncementEventPlay(
            text,
            std::lerp(first_volume, last_volume, announcement_factor),
            output_device_,
            this);
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
