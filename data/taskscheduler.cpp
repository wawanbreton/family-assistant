#include "taskscheduler.h"

#include <QJsonArray>
#include <QJsonObject>

#include "data/activetask.h"
#include "data/duetask.h"
#include "data/kid.h"
#include "data/kidmanager.h"
#include "preferences.h"


SINGLETON_IMPL(TaskScheduler)
SINGLETON_DESTRUCTOR_IMPL(TaskScheduler)

TaskScheduler::TaskScheduler(QObject* parent)
    : QObject{ parent }
{
    Preferences::access()->registerPreference(PreferenceEntry::TaskSoonDelay, QMetaType::Int, 120);
    Preferences::access()->registerPreference(PreferenceEntry::TaskInProgressDelay, QMetaType::Int, 60);
    Preferences::access()->registerPreference(PreferenceEntry::TaskCloseToEndDelay, QMetaType::Int, 10);
}

void TaskScheduler::load(const QJsonObject& json_object)
{
    auto iterator = json_object.constFind("tasks");
    if (iterator != json_object.constEnd())
    {
        QJsonArray tasks_array = iterator.value().toArray();
        for (const QJsonValue& task_value : tasks_array)
        {
            auto active_task = new ActiveTask(this);
            active_task->load(task_value.toObject());
            active_tasks_ << active_task;
        }
    }
    else
    {
        qWarning() << "No kids defined in data";
    }
}

void TaskScheduler::start(bool reset_tasks)
{
    if (reset_tasks)
    {
        for (Kid* kid : KidManager::access()->getKids())
        {
            kid->getTasks()->clear();
        }

        spawnDueTasks();
    }

    scheduleNextTrigger();
}

void TaskScheduler::spawnDueTasks()
{
    const auto now = QDateTime::currentDateTime();
    const QDate current_date = now.date();
    const auto current_day = static_cast<DayOfWeek::Enum>(current_date.dayOfWeek());

    for (const ActiveTask* active_task : active_tasks_)
    {
        for (const TaskOccurence& occurence : active_task->getOccurences())
        {
            if (occurence.day == current_day)
            {
                for (Kid* kid : KidManager::access()->getKids())
                {
                    const QDateTime due_timestamp(current_date, occurence.time, Qt::LocalTime);

                    if (due_timestamp > now)
                    {
                        auto due_task = new DueTask();
                        due_task->copyFrom(active_task);
                        due_task->setDueTimestamp(due_timestamp);
                        kid->getTasks()->append(due_task);
                    }
                }
            }
        }
    }
}

void TaskScheduler::scheduleNextTrigger()
{
}
