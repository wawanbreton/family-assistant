#include "taskscheduler.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

#include "data/activetask.h"
#include "data/duetask.h"
#include "data/kid.h"
#include "data/kidmanager.h"
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
    auto iterator = json_object.constFind("tasks");
    if (iterator != json_object.constEnd())
    {
        QJsonArray tasks_array = iterator.value().toArray();
        for (const QJsonValue& task_value : tasks_array)
        {
            auto active_task = new ActiveTask(this);
            active_task->load(task_value.toObject());
            tasks_ << active_task;
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
            kid->clearTasks();
        }

        spawnDueTasks();
    }
    else
    {
        scheduleNextTrigger();
    }
}

QList<const ActiveTask*> TaskScheduler::getCasualTasks() const
{
    QList<const ActiveTask*> casual_tasks;

    std::copy_if(
        tasks_.begin(),
        tasks_.end(),
        std::back_inserter(casual_tasks),
        [](const ActiveTask* task) { return task->isCasual(); });

    return casual_tasks;
}

const ActiveTask* TaskScheduler::findTask(const QUuid& uuid) const
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
    const QList<Kid*> kids = KidManager::access()->getKids();
    auto iterator = std::find_if(kids.begin(), kids.end(), [task](const Kid* kid) { return kid->hasCasualTask(task); });

    if (iterator != kids.end())
    {
        auto due_task = new DueTask(this);
        due_task->setTask(task);
        const auto duration = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::CasualTaskDuration));
        due_task->setDueTimestamp(QDateTime::currentDateTime().addDuration(duration));
        (*iterator)->addTask(due_task);

        qInfo() << "add casual task" << task->getDesc() << "to" << (*iterator)->getName();
    }
    else
    {
        qWarning() << "Task" << task->getDesc() << "is not affected to a kid";
    }
}

void TaskScheduler::spawnDueTasks()
{
    qInfo() << "Spawn due tasks";

    const auto now = QDateTime::currentDateTime();
    const QDate current_date = now.date();
    const auto current_day = static_cast<DayOfWeek::Enum>(current_date.dayOfWeek());

    for (const ActiveTask* active_task : tasks_)
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
                        due_task->setTask(active_task);
                        due_task->setDueTimestamp(due_timestamp);
                        kid->addTask(due_task);
                    }
                }
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
