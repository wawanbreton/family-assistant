#pragma once

#include <QObject>

class AbstractAnnouncementEvent : public QObject
{
    Q_OBJECT

public:
    explicit AbstractAnnouncementEvent(QObject* parent = nullptr);

    virtual void start() = 0;

signals:
    void over();
};
