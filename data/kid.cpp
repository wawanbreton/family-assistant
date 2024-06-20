#include "kid.h"

#include <QJsonArray>
#include <QJsonObject>

#include "data/task.h"
#include "utils/json.h"


Kid::Kid(QObject* parent)
    : QObject{ parent }
{
}

void Kid::load(const QJsonObject& json_object)
{
    Json::mapPropertyToObject(json_object, this);

    auto iterator = json_object.constFind("tasks");
    if (iterator != json_object.constEnd())
    {
        QJsonArray tasks_array = iterator.value().toArray();
        for (const QJsonValue& task_object : tasks_array)
        {
            auto task = new Task(this);
            task->load(task_object.toObject());
            tasks_ << task;
        }
    }
}

const QString& Kid::getName() const
{
    return name_;
}

void Kid::setName(const QString& name)
{
    if (name != name_)
    {
        name_ = name;
        emit nameChanged(name_);
    }
}

QQmlListProperty<Task> Kid::getTasks()
{
    return QQmlListProperty(this, &tasks_);
}
