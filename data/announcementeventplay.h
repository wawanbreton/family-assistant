#pragma once

#include <QAudio>

#include "data/abstractannouncementevent.h"

class AnnouncementEventPlay : public AbstractAnnouncementEvent
{
    Q_OBJECT

public:
    explicit AnnouncementEventPlay(const QString& text, QObject* parent = nullptr);

    virtual ~AnnouncementEventPlay();

    void start() override;

private:
    void playAnnouncement();

    void onGenerateWavFileDone(bool success);

    void onAudioSinkStateChanged(QAudio::State state);

    static bool generateWavFile(const QString& text, const QString& wav_filepath);

private:
    const QString text_;
    QString wav_filepath_;
};
