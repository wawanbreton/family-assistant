#pragma once

#include "data/abstractannouncementevent.h"

class AnnouncementEventWait : public AbstractAnnouncementEvent
{
    Q_OBJECT

public:
    explicit AnnouncementEventWait(const std::chrono::milliseconds delay, QObject* parent = nullptr);

    void start() override;

private:
    const std::chrono::milliseconds delay_;
};