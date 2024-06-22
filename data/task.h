#pragma once

#include <QObject>
#include <QTime>

#include "data/taskstate.h"

class QTimer;

class Task : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString desc READ getDesc WRITE setDesc NOTIFY dummyChanged)
    Q_PROPERTY(QDateTime due_timestamp READ getDueTimestamp WRITE setDueTimestamp NOTIFY dueTimestampChanged)
    Q_PROPERTY(QString due_time_str READ getDueTimeStr NOTIFY dueTimestampChanged)
    Q_PROPERTY(QString icon WRITE setIcon)
    Q_PROPERTY(QString icon_path READ getIconPath NOTIFY dummyChanged)
    Q_PROPERTY(TaskState::Enum state READ getState NOTIFY stateChanged)

public:
    explicit Task(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getDesc() const;

    void setDesc(const QString& desc);

    const QDateTime& getDueTimestamp() const;

    void setDueTimestamp(const QDateTime& due_timestamp);

    QString getDueTimeStr() const;

    const QString& getIconPath() const;

    void setIcon(const QString& icon);

    TaskState::Enum getState() const;

    Q_INVOKABLE void setAccomplished();

signals:
    void dueTimestampChanged();

    void stateChanged();

    void accomplished();

    void dummyChanged();

private:
    void updateState();

private:
    QString desc_;
    QDateTime due_timestamp_;
    QString icon_path_;
    TaskState::Enum state_{ TaskState::Early };
    QTimer* const timer_next_update_;
};
