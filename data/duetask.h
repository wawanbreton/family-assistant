#pragma once

#include <QTime>

#include "data/task.h"
#include "data/taskstate.h"

class QTimer;

class DueTask : public Task
{
    Q_OBJECT
    Q_PROPERTY(QDateTime due_timestamp READ getDueTimestamp WRITE setDueTimestamp NOTIFY dueTimestampChanged)
    Q_PROPERTY(QString due_time_str READ getDueTimeStr NOTIFY dueTimestampChanged)
    Q_PROPERTY(TaskState::Enum state READ getState NOTIFY stateChanged)

public:
    explicit DueTask(QObject* parent = nullptr);

    virtual void copyFrom(const Task* other) override;

    virtual void load(const QJsonObject& json_object) override;

    const QDateTime& getDueTimestamp() const;

    void setDueTimestamp(const QDateTime& due_timestamp);

    QString getDueTimeStr() const;

    TaskState::Enum getState() const;

    Q_INVOKABLE void setAccomplished();

signals:
    void dueTimestampChanged();

    void stateChanged();

    void accomplished();

private:
    void updateState();

private:
    QDateTime due_timestamp_;
    TaskState::Enum state_{ TaskState::Early };
    QTimer* const timer_next_update_;
};
