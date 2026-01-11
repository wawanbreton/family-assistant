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
    Q_PROPERTY(QList<ActiveTask*> casual_tasks READ getCasualTasks NOTIFY casualTasksChanged)

public:
    void load(const QJsonObject& json_object);

    void start(bool reset_tasks);

    QList<ActiveTask*> getCasualTasks() const;

    ActiveTask* findTask(const QUuid& uuid) const;

    Q_INVOKABLE void appendCasualTask(ActiveTask* task);

signals:
    void casualTasksChanged();

private:
    void spawnDueTasks();

    void scheduleNextTrigger();

private:
    QList<ActiveTask*> tasks_;
    QTimer* const timer_spawn_tasks_;
};
