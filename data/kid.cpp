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
            addTask(task);
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

const QObjectList Kid::getTasks() const
{
    QObjectList list;
    for (Task* task : tasks_)
    {
        list << task;
    }
    return list;
}

void Kid::addTask(Task* task)
{
    task->setParent(this);

    connect(
        task,
        &Task::accomplished,
        this,
        [this, task]()
        {
            onTaskAccomplished(task);
        });

    tasks_ << task;
}

void Kid::onTaskAccomplished(Task* task)
{
    tasks_.removeOne(task);
    task->deleteLater();
    emit tasksChanged();
}
