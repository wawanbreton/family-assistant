#include "taskscheduler.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/qobject_helper.h>

#include "data/announcementmanager.h"
#include "data/duetask.h"
#include "data/kid.h"
#include "data/preferences.h"
#include "data/usermanager.h"


SINGLETON_IMPL(TaskScheduler)
SINGLETON_DESTRUCTOR_IMPL(TaskScheduler)

TaskScheduler::TaskScheduler(QObject* parent)
    : QObject{ parent }
    , timer_spawn_tasks_(new QTimer(this))
    , timer_trigger_announcements_(new QTimer(this))
{
    Preferences::access()->registerPreference(PreferenceEntry::TaskSoonDelay, QMetaType::Int, 120);
    Preferences::access()->registerPreference(PreferenceEntry::TaskInProgressDelay, QMetaType::Int, 60);
    Preferences::access()->registerPreference(PreferenceEntry::TaskCloseToEndDelay, QMetaType::Int, 10);
    Preferences::access()->registerPreference(PreferenceEntry::CasualTaskDuration, QMetaType::Int, 60);
    Preferences::access()->registerPreference(PreferenceEntry::WeeklyScreenTime, QMetaType::Int, 7 * 30);

    timer_spawn_tasks_->setSingleShot(true);
    connect(timer_spawn_tasks_, &QTimer::timeout, this, [this] { spawnDueTasks(false); });

    timer_trigger_announcements_->setSingleShot(true);
    timer_trigger_announcements_->setTimerType(Qt::PreciseTimer);
    connect(timer_trigger_announcements_, &QTimer::timeout, this, &TaskScheduler::triggerAnnouncements);
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

void TaskScheduler::start(const bool reset_tasks, const bool force_reset_screen_time)
{
    if (reset_tasks)
    {
        spawnDueTasks(force_reset_screen_time);
    }
    else
    {
        scheduleNextTrigger();
        scheduleNextAnnouncement();
    }

    for (Kid* kid : UserManager::access()->getKids())
    {
        connect(kid->getTasks(), &TasksModel::changed, this, &TaskScheduler::scheduleNextAnnouncement);
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
    const auto due_timestamp = QDateTime::currentDateTime() + duration;
    appendDueTask(task, due_timestamp);
}

void TaskScheduler::spawnDueTasks(bool force_reset_screen_time)
{
    qInfo() << "Spawn due tasks";

    const auto now = QDateTime::currentDateTime();
    const QDate current_date = now.date();
    const auto current_day = static_cast<DayOfWeek::Enum>(current_date.dayOfWeek());
    const bool reset_screen_times = force_reset_screen_time || current_day == DayOfWeek::Monday;
    const auto screen_time = std::chrono::minutes(Preferences::get()->getInt(PreferenceEntry::WeeklyScreenTime));

    for (Kid* kid : UserManager::access()->getKids())
    {
        kid->clearTasks();

        if (reset_screen_times)
        {
            kid->resetScreenTime(screen_time);
        }
    }

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
    scheduleNextAnnouncement();
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

void TaskScheduler::scheduleNextAnnouncement()
{
    std::optional<QDateTime> next_announcement;
    for (const Kid* kid : UserManager::get()->getKids())
    {
        for (const DueTask* due_task : kid->getTasks()->getTasks())
        {
            for (const QDateTime& announcement : due_task->getAnnouncementTimestamps())
            {
                if (! next_announcement.has_value() || announcement < next_announcement)
                {
                    next_announcement = announcement;
                }
            }
        }
    }

    if (next_announcement.has_value())
    {
        const auto now = QDateTime::currentDateTime();
        qint64 delta_time = now.msecsTo(*next_announcement);

        qInfo() << "Trigger announcement in" << delta_time << "ms" << (*next_announcement);

        timer_trigger_announcements_->start(delta_time);
    }
    else
    {
        timer_trigger_announcements_->stop();
    }
}

void TaskScheduler::triggerAnnouncements()
{
    struct AnnouncementKey
    {
        const ActiveTask* task;
        std::chrono::minutes delay;

        bool operator<(const AnnouncementKey& other) const
        {
            return other.task == task ? delay < other.delay : task < other.task;
        }
    };

    QMap<AnnouncementKey, QStringList> announcements;
    const auto now = QDateTime::currentDateTimeUtc();
    for (const Kid* kid : UserManager::get()->getKids())
    {
        for (DueTask* due_task : kid->getTasks()->getTasks())
        {
            std::optional<std::chrono::minutes> delay = due_task->popAnnouncement(now);
            if (delay.has_value())
            {
                announcements[AnnouncementKey{ due_task->getTask(), *delay }].append(kid->getPronouncedName());
            }
        }
    }


    for (auto iterator = announcements.begin(); iterator != announcements.end(); ++iterator)
    {
        const ActiveTask* task = iterator.key().task;
        std::chrono::minutes delay = iterator.key().delay;
        const QStringList& kids = iterator.value();

        QStringList first_kids = kids.mid(0, kids.length() - 1);
        QString last_kid = kids.last();

        QString kids_ponctuated;
        if (first_kids.isEmpty())
        {
            kids_ponctuated = last_kid;
        }
        else
        {
            kids_ponctuated = QStringList({ first_kids.join(", "), last_kid }).join(" et ");
        }

        QString announcement;
        if (delay < 0s)
        {
            announcement = QString("%1, dans %2 minutes, il sera trop tard pour %3")
                               .arg(
                                   kids_ponctuated,
                                   delay.count() == -1 ? "une" : QString::number(-delay.count()),
                                   task->getDesc());
        }
        else if (delay > 0s)
        {
            announcement = QString("%1, le délai pour %2 est dépassé de %3 minutes")
                               .arg(
                                   kids_ponctuated,
                                   task->getDesc(),
                                   delay.count() == 1 ? "une" : QString::number(delay.count()));
        }
        else
        {
            announcement = QString("%1, il est trop tard pour %2").arg(kids_ponctuated, task->getDesc());
        }

        AnnouncementManager::access()->addAnnouncement(announcement);
    }

    scheduleNextAnnouncement();
}
