#include "taskscheduler.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/qobject_helper.h>

#include "data/duetask.h"
#include "data/kid.h"
#include "data/usermanager.h"
#include "preferences.h"


SINGLETON_IMPL(TaskScheduler)
SINGLETON_DESTRUCTOR_IMPL(TaskScheduler)

TaskScheduler::TaskScheduler(QObject* parent)
    : QObject{ parent }
    , timer_spawn_tasks_(new QTimer(this))
{
    Preferences::access()->registerPreference(PreferenceEntry::TaskSoonDelay, QMetaType::Int, 120);
    Preferences::access()->registerPreference(PreferenceEntry::TaskInProgressDelay, QMetaType::Int, 60);
    Preferences::access()->registerPreference(PreferenceEntry::TaskCloseToEndDelay, QMetaType::Int, 10);
    Preferences::access()->registerPreference(PreferenceEntry::CasualTaskDuration, QMetaType::Int, 60);

    timer_spawn_tasks_->setSingleShot(true);
    connect(timer_spawn_tasks_, &QTimer::timeout, this, &TaskScheduler::spawnDueTasks);
}

void TaskScheduler::load(const QJsonObject& json_object)
{
    QList<QJsonObject> tasks_objects
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "tasks", __METHOD__);
    for (const QJsonObject& task_object : tasks_objects)
    {
        ActiveTask* active_task = new ActiveTask(this);
        active_task->load(task_object);
        tasks_ << active_task;
    }
}

void TaskScheduler::save(QJsonObject& json_object) const
{
    QJsonArray tasks_array;

    for (const ActiveTask* task : tasks_)
    {
        QJsonObject task_object;
        task->save(task_object);
        tasks_array.append(task_object);
    }

    json_object["tasks"] = tasks_array;
}

void TaskScheduler::start(bool reset_tasks)
{
    if (reset_tasks)
    {
        spawnDueTasks();
    }
    else
    {
        scheduleNextTrigger();
    }
}

const QList<ActiveTask*>& TaskScheduler::getTasks() const
{
    return tasks_;
}

ActiveTask* TaskScheduler::findTask(const QUuid& uuid) const
{
    auto iterator = std::find_if(
        tasks_.begin(),
        tasks_.end(),
        [&uuid](const ActiveTask* task) { return task->getUuid() == uuid; });
    if (iterator != tasks_.end())
    {
        return *iterator;
    }

    return nullptr;
}

void TaskScheduler::appendCasualTask(const ActiveTask* task)
{
    const auto duration = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::CasualTaskDuration));
    const auto due_timestamp = QDateTime::currentDateTime().addDuration(duration);
    appendDueTask(task, due_timestamp);
}


void TaskScheduler::spawnDueTasks()
{
    qInfo() << "Spawn due tasks";

    for (Kid* kid : UserManager::access()->getKids())
    {
        kid->clearTasks();
    }

    const auto now = QDateTime::currentDateTime();
    const QDate current_date = now.date();
    const auto current_day = static_cast<DayOfWeek::Enum>(current_date.dayOfWeek());

    for (const ActiveTask* active_task : tasks_)
    {
        for (const TaskOccurences& occurences : active_task->getOccurences())
        {
            if (! occurences.days.contains(current_day))
            {
                continue;
            }

            for (const QTime& occurence_time : occurences.times)
            {
                const QDateTime due_timestamp(current_date, occurence_time, Qt::LocalTime);
                if (due_timestamp <= now)
                {
                    continue;
                }

                appendDueTask(active_task, due_timestamp);
            }
        }
    }

    scheduleNextTrigger();
}

void TaskScheduler::scheduleNextTrigger()
{
    const auto now = QDateTime::currentDateTime();
    const auto next_trigger = QDateTime(now.date().addDays(1), QTime(0, 0, 1), Qt::LocalTime);
    qint64 delta_time = now.msecsTo(next_trigger);

    qInfo() << "Spawning new tasks in" << delta_time << "ms";

    timer_spawn_tasks_->start(delta_time);
}

void TaskScheduler::appendDueTask(const ActiveTask* task, const QDateTime& due_timestamp)
{
    QList<Kid*> kids;
    if (task->getAffectedKid())
    {
        kids << task->getAffectedKid();
    }
    else
    {
        kids = UserManager::access()->getKids();
    }

    for (Kid* kid : kids)
    {
        auto due_task = new DueTask(this);
        due_task->setTask(task);
        due_task->setDueTimestamp(due_timestamp);
        kid->addTask(due_task);

        qInfo() << "Add task" << task->getDesc() << "to" << kid->getName();
    }
}
