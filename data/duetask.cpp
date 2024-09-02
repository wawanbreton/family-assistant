#include "duetask.h"

#include <QTimer>
#include <easyqt/datastorage.h>

#include "data/preferences.h"
#include "utils/json.h"


using namespace std::chrono_literals;

DueTask::DueTask(QObject* parent)
    : Task{ parent }
    , timer_next_update_(new QTimer(this))
{
    timer_next_update_->setSingleShot(true);
    connect(timer_next_update_, &QTimer::timeout, this, &DueTask::updateState);

#warning Make this smarter by updating only of the concerned entry changed
    connect(Preferences::get(), &Preferences::valueChanged, this, &DueTask::updateState);
}

void DueTask::copyFrom(const Task* other)
{
    Task::copyFrom(other);
}

void DueTask::load(const QJsonObject& json_object)
{
    Task::load(json_object);
    updateState();
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
    const auto soon_delay = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::TaskSoonDelay));
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
