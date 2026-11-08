#pragma once

#include <QQueue>
#include <chrono>
#include <easyqt/singleton.h>

using namespace std::chrono_literals;

struct piper_synthesizer;

class AbstractAnnouncementEvent;

class AnnouncementManager : public QObject
{
    Q_OBJECT
    SINGLETON_WITH_1_PARAM(AnnouncementManager, const bool skip_voice_engine)

public:
    void addAnnouncement(
        const QString& text,
        const std::optional<quint8> repeat = std::nullopt,
        const std::chrono::seconds delay = 2s);

    struct piper_synthesizer* getVoiceSynthesizer();

private:
    void onEventOver();

    void startNextEvent();

private:
    QQueue<AbstractAnnouncementEvent*> events_queue_;
    struct piper_synthesizer* voice_synthesizer_{ nullptr };
    int repeat_{ 1 };
};
