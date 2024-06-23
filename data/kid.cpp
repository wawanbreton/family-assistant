#include "kid.h"

#include <QJsonArray>
#include <QJsonObject>

#include "data/task.h"
#include "data/tasksmodel.h"
#include "utils/json.h"


Kid::Kid(QObject* parent)
    : QObject{ parent }
    , tasks_(new TasksModel(this))
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

TasksModel* Kid::getTasks()
{
    return tasks_;
}

quint32 Kid::getPoints() const
{
    return points_;
}

void Kid::setPoints(const quint32 points)
{
    if (points != points_)
    {
        const qint32 delta = points - points_;
        points_ = points;
        emit pointsChanged(points_, delta);
    }
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

    tasks_->append(task);
}

void Kid::onTaskAccomplished(Task* task)
{
    tasks_->remove(task);
    task->deleteLater();
}
