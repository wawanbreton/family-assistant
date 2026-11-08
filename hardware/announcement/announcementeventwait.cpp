#include "announcementeventwait.h"

#include <QTimer>


AnnouncementEventWait::AnnouncementEventWait(const std::chrono::milliseconds delay, QObject* parent)
    : AbstractAnnouncementEvent{ parent }
    , delay_(delay)
{
}

void AnnouncementEventWait::start()
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(delay_);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &AnnouncementEventWait::over);
    timer->start();
}
