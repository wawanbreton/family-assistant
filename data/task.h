#pragma once

#include <QObject>
#include <QTime>

#include "data/taskstate.h"

class Task : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString desc READ getDesc WRITE setDesc)
    Q_PROPERTY(QDateTime due_timestamp READ getDueTimestamp WRITE setDueTimestamp NOTIFY dueTimestampChanged)
    Q_PROPERTY(QString due_time_str READ getDueTimeStr NOTIFY dueTimestampChanged)
    Q_PROPERTY(QString icon WRITE setIcon)
    Q_PROPERTY(QString icon_path READ getIconPath)
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

signals:
    void dueTimestampChanged();

    void stateChanged();

private:
    void updateState();

private:
    QString desc_;
    QDateTime due_timestamp_;
    QString icon_path_;
    TaskState::Enum state_{ TaskState::Early };
};
