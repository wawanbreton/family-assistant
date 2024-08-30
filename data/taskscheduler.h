#pragma once

#include <QObject>
#include <easyqt/singleton.h>

class ActiveTask;

class TaskScheduler : public QObject
{
    Q_OBJECT
    SINGLETON(TaskScheduler)

public:
    void load(const QJsonObject& json_object);

private:
    QList<ActiveTask*> active_tasks_;
};
