#include "audiomanager.h"

#include <QMediaDevices>

#include "data/preferences.h"


AudioManager::AudioManager(
    const PreferenceEntry::Enum output_device_preference,
    const QString& audio_type,
    QObject* parent)
    : QObject{ parent }
    , output_device_(QMediaDevices::defaultAudioOutput())
{
    Preferences::access()->registerPreference(output_device_preference, QMetaType::QString, "");

    const QString announcement_audio_device_name = Preferences::get()->getString(output_device_preference).trimmed();
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
            qInfo() << "Playing" << audio_type << "on" << output_device_.description();
        }
        else
        {
            qWarning() << "Output device" << announcement_audio_device_name << "for" << audio_type
                       << "was not found, using default" << output_device_.description();
        }
    }
    else
    {
        qInfo() << "Playing" << audio_type << "on default output" << output_device_.description();
    }
}

QAudioDevice& AudioManager::getOutputDevice()
{
    return output_device_;
}
