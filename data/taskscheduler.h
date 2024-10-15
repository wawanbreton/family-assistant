#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <easyqt/singleton.h>

class QTimer;

class ActiveTask;
class Kid;
class Task;

class TaskScheduler : public QObject
{
    Q_OBJECT
    SINGLETON(TaskScheduler)
    Q_PROPERTY(QList<const ActiveTask*> casual_tasks READ getCasualTasks CONSTANT)

public:
    void load(const QJsonObject& json_object);

    void start(bool reset_tasks);

    QList<const ActiveTask*> getCasualTasks() const;

    const ActiveTask* findTask(const QUuid& uuid) const;

    Q_INVOKABLE void appendCasualTask(const ActiveTask* task);

private:
    void spawnDueTasks();

    void scheduleNextTrigger();

private:
    QList<const ActiveTask*> tasks_;
    QTimer* const timer_spawn_tasks_;
};
