#pragma once

#include <QAudioDevice>

#include "data/preferenceentry.h"

class AudioManager : public QObject
{
    Q_OBJECT

protected:
    AudioManager(const PreferenceEntry::Enum output_device_preference, const QString& audio_type, QObject* parent);

    QAudioDevice& getOutputDevice();

private:
    QAudioDevice output_device_;
};
