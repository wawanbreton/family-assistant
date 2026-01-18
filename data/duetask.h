#pragma once

#include <QTime>

#include "data/activetask.h"
#include "data/taskstate.h"

class QTimer;

class TaskScheduler;

class DueTask : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime due_timestamp READ getDueTimestamp WRITE setDueTimestamp NOTIFY dueTimestampChanged)
    Q_PROPERTY(QString due_time_str READ getDueTimeStr NOTIFY dueTimestampChanged)
    Q_PROPERTY(TaskState::Enum state READ getState NOTIFY stateChanged)
    Q_PROPERTY(const ActiveTask* task READ getTask CONSTANT)

public:
    explicit DueTask(QObject* parent = nullptr);

    bool load(const QJsonObject& json_object);

    void save(QJsonObject& json_object) const;

    const QDateTime& getDueTimestamp() const;

    void setDueTimestamp(const QDateTime& due_timestamp);

    QString getDueTimeStr() const;

    TaskState::Enum getState() const;

    const ActiveTask* getTask() const;

    void setTask(const ActiveTask* task);

    Q_INVOKABLE void setAccomplished();

signals:
    void dueTimestampChanged();

    void stateChanged();

    void accomplished();

private:
    void updateState();

private:
    const ActiveTask* task_{ nullptr };
    QDateTime due_timestamp_;
    TaskState::Enum state_{ TaskState::Early };
    QTimer* const timer_next_update_;
};
