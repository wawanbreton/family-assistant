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
    Q_PROPERTY(QList<ActiveTask*> tasks READ getTasks NOTIFY tasksChanged)

public:
    void load(const QJsonObject& json_object);

    void save(QJsonObject& json_object) const;

    void start(bool reset_tasks);

    const QList<ActiveTask*>& getTasks() const;

    ActiveTask* findTask(const QUuid& uuid) const;

    Q_INVOKABLE void appendCasualTask(const ActiveTask *task);

signals:
    void tasksChanged();

private:
    void spawnDueTasks();

    void scheduleNextTrigger();

    void appendDueTask(const ActiveTask *active_task, const QDateTime& due_timestamp);

private:
    QList<ActiveTask*> tasks_;
    QTimer* const timer_spawn_tasks_;
};
