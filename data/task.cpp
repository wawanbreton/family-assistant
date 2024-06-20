#include "task.h"

#include <easyqt/datastorage.h>

#include "utils/json.h"


Task::Task(QObject* parent)
    : QObject{ parent }
{
}

void Task::load(const QJsonObject& json_object)
{
    Json::mapPropertyToObject(json_object, this);

    qDebug() << desc_ << due_timestamp_;
}

const QString& Task::getDesc() const
{
    return desc_;
}

void Task::setDesc(const QString& desc)
{
    desc_ = desc;
}

const QDateTime& Task::getDueTimestamp() const
{
    return due_timestamp_;
}

void Task::setDueTimestamp(const QDateTime& due_timestamp)
{
    if (due_timestamp != due_timestamp_)
    {
        due_timestamp_ = due_timestamp;
        emit dueTimestampChanged();
    }
}

QString Task::getDueTimeStr() const
{
    return due_timestamp_.toLocalTime().toString("HH:mm");
}

const QString& Task::getIconPath() const
{
    return icon_path_;
}

void Task::setIcon(const QString& icon)
{
    icon_path_ = easyqt::DataStorage::findResource(icon, easyqt::DataStorage::ResourceType::Icon, { "png", "svg" });
}
