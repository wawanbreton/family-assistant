#pragma once

#include <QObject>
#include <easyqt/singleton.h>

class QTimer;

class ActiveTask;

class TaskScheduler : public QObject
{
    Q_OBJECT
    SINGLETON(TaskScheduler)

public:
    void load(const QJsonObject& json_object);

    void start(bool reset_tasks);

private:
    void spawnDueTasks();

    void scheduleNextTrigger();

private:
    QList<ActiveTask*> active_tasks_;
    QTimer* const timer_spawn_tasks_;
};
