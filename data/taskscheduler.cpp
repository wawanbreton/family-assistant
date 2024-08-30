#include "taskscheduler.h"

#include <QJsonArray>
#include <QJsonObject>

#include "data/activetask.h"
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
