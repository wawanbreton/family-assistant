#include "task.h"

#include <easyqt/datastorage.h>

#include "data/preferences.h"
#include "utils/json.h"


Task::Task(QObject* parent)
    : QObject{ parent }
{
    updateState();

#warning Make this smarter by updating only of the concerned entry changed
    connect(Preferences::get(), &Preferences::valueChanged, this, &Task::updateState);
}

void Task::load(const QJsonObject& json_object)
{
    Json::mapPropertyToObject(json_object, this);
    updateState();
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

TaskState::Enum Task::getState() const
{
    return state_;
}

void Task::setAccomplished()
{
    emit accomplished();
}

void Task::updateState()
{
    const auto in_progress_delay
        = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::TaskInProgressDelay));
    const auto soon_delay = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::TaskSoonDelay));
    const auto close_to_end_delay
        = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::TaskCloseToEndDelay));

    const auto now = QDateTime::currentDateTimeUtc();
    const auto delta = std::chrono::duration_cast<std::chrono::minutes>(due_timestamp_ - now);

    TaskState::Enum new_state;

    if (delta >= soon_delay)
    {
        new_state = TaskState::Early;
    }
    else if (delta >= in_progress_delay)
    {
        new_state = TaskState::Soon;
    }
    else if (delta >= close_to_end_delay)
    {
        new_state = TaskState::InProgress;
    }
    else if (delta.count() >= 0)
    {
        new_state = TaskState::CloseToEnd;
    }
    else
    {
        new_state = TaskState::Late;
    }

    if (new_state != state_)
    {
        state_ = new_state;
        emit stateChanged();
    }
}
