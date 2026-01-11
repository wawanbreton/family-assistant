#include "activetask.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/activecasualtask.h"
#include "data/activerecurringtask.h"


ActiveTask::ActiveTask(QObject* parent)
    : Task{ parent }
{
}

ActiveTask::ActiveTask(const ActiveTask& other)
    : Task(other)
{
}

const QUuid& ActiveTask::getUuid() const
{
    return uuid_;
}

void ActiveTask::load(const QJsonObject& json_object)
{
    Task::load(json_object);

    uuid_ = Json::loadValue(json_object, "uuid", __METHOD__, QUuid());
}

ActiveTask* ActiveTask::makeAndLoad(const QJsonObject& json_object, QObject* parent)
{
    ActiveTask* task = nullptr;

    if (json_object.contains("due_times"))
    {
        task = new ActiveRecurringTask(parent);
    }
    else if (json_object.contains("affected_kid_uuid"))
    {
        task = new ActiveCasualTask(parent);
    }
    else
    {
        qWarning() << "JSON node is missing a distinctive attribute to instanciate the proper task:"
                   << json_object.keys();
    }

    if (task)
    {
        task->load(json_object);
    }

    return task;
}
