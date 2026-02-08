#include "duetask.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>
#include <easyqt/datastorage.h>
#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/activetask.h"
#include "data/preferences.h"
#include "data/taskscheduler.h"


using namespace std::chrono_literals;

DueTask::DueTask(QObject* parent)
    : QObject{ parent }
    , timer_next_update_(new QTimer(this))
{
    timer_next_update_->setSingleShot(true);
    connect(timer_next_update_, &QTimer::timeout, this, &DueTask::updateState);

#warning Make this smarter by updating only of the concerned entry changed
    connect(Preferences::get(), &Preferences::valueChanged, this, &DueTask::updateState);
}

bool DueTask::load(const QJsonObject& json_object)
{
    due_timestamp_ = easyqt::Json::loadProperty(json_object, "due_timestamp", __METHOD__, due_timestamp_);

    const std::optional<QUuid> task_uuid = easyqt::Json::loadProperty<QUuid>(json_object, "task_uuid", __METHOD__);
    if (task_uuid.has_value())
    {
        task_ = TaskScheduler::get()->findTask(*task_uuid);
        if (! task_)
        {
            qWarning() << "No task defined with UUID" << *task_uuid;
        }
    }

    return ! due_timestamp_.isNull() && task_;
}

void DueTask::save(QJsonObject& json_object) const
{
    json_object["due_timestamp"] = easyqt::Json::saveValue(due_timestamp_);

    if (task_)
    {
        json_object["task_uuid"] = easyqt::Json::saveValue(task_->getUuid());
    }
}

const QDateTime& DueTask::getDueTimestamp() const
{
    return due_timestamp_;
}

void DueTask::setDueTimestamp(const QDateTime& due_timestamp)
{
    if (due_timestamp != due_timestamp_)
    {
        due_timestamp_ = due_timestamp;
        updateState();
        emit dueTimestampChanged();
    }
}

QString DueTask::getDueTimeStr() const
{
    return due_timestamp_.toLocalTime().toString("HH:mm");
}

TaskState::Enum DueTask::getState() const
{
    return state_;
}

const ActiveTask* DueTask::getTask() const
{
    return task_;
}

void DueTask::setTask(const ActiveTask* desc)
{
    task_ = desc;
}

int DueTask::currentReward() const
{
    int reward = task_->getReward();

    if (state_ == TaskState::Late)
    {
        reward /= 3;
    }

    return reward;
}

void DueTask::setAccomplished()
{
    emit accomplished();
}

void DueTask::updateState()
{
    if (! due_timestamp_.isValid())
    {
        return;
    }

    // Get delays stored in preferences
    std::optional<int> specific_activation_delay = task_->getActivationDelay();
    const auto soon_delay = std::chrono::minutes(
        specific_activation_delay.value_or(Preferences::get()->getInt(PreferenceEntry::TaskSoonDelay)));
    const auto in_progress_delay
        = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::TaskInProgressDelay));
    const auto close_to_end_delay
        = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::TaskCloseToEndDelay));

    // Calculate time delta
    const auto now = QDateTime::currentDateTimeUtc();
    const auto delta = due_timestamp_ - now;

    // Set new state according to delta
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

    // Update state if needed
    if (new_state != state_)
    {
        state_ = new_state;
        emit stateChanged();
    }

    // Setup timer for next update
    timer_next_update_->stop();
    std::optional<std::chrono::minutes> next_update_delay;
    switch (state_)
    {
        case TaskState::Early:
            next_update_delay = soon_delay;
            break;
        case TaskState::Soon:
            next_update_delay = in_progress_delay;
            break;
        case TaskState::InProgress:
            next_update_delay = close_to_end_delay;
            break;
        case TaskState::CloseToEnd:
            next_update_delay = std::chrono::minutes::zero();
            break;
        case TaskState::Late:
            break;
    }

    if (next_update_delay)
    {
        const QDateTime next_update_timestamp = due_timestamp_.addDuration(-next_update_delay.value());
        std::chrono::milliseconds delay_next_update = next_update_timestamp - now;
        delay_next_update = std::max(std::chrono::milliseconds::zero(), delay_next_update);
        timer_next_update_->start(delay_next_update + 1ms);
    }
}
