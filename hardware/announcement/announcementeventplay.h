#pragma once

#include <QMediaPlayer>

#include "data/abstractannouncementevent.h"

class AnnouncementEventPlay : public AbstractAnnouncementEvent
{
    Q_OBJECT

public:
    explicit AnnouncementEventPlay(const QString& text, const double volume, QObject* parent = nullptr);

    void start() override;

private:
    void playAnnouncement();

    void onGenerateWavFileDone(bool success);

    void onMediaPlayerStatusChanged(QMediaPlayer::MediaStatus status);

    static bool generateWavFile(const QString& text, const QString& wav_filepath);

private:
    const QString text_;
    const double volume_;
    QString wav_filepath_;
};
