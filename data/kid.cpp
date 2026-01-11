#include "kid.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/parser.h>

#include "data/duetask.h"
#include "data/taskscheduler.h"
#include "data/tasksmodel.h"
#include "data/theme.h"


Kid::Kid(QObject* parent)
    : QObject{ parent }
    , tasks_(new TasksModel(this))
    , theme_(new Theme(this))
    , uuid_(QUuid::createUuid())
{
}

void Kid::load(const QJsonObject& json_object)
{
    Json::mapValuesToObjectProperties(json_object, this);

    uuid_ = Json::loadValue(json_object, "uuid", __METHOD__, QUuid());

    auto iterator = json_object.constFind("tasks");
    if (iterator != json_object.constEnd())
    {
        QJsonArray tasks_array = iterator.value().toArray();
        for (const QJsonValue& task_object : tasks_array)
        {
            auto task = new DueTask(this);
            if (task->load(task_object.toObject()))
            {
                addTask(task);
            }
            else
            {
                delete task;
            }
        }
    }

    iterator = json_object.constFind("theme");
    if (iterator != json_object.constEnd())
    {
        theme_->load(iterator.value().toObject());
    }
}

const QUuid& Kid::getUuid() const
{
    return uuid_;
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

bool Kid::hasTasks() const
{
    return tasks_->rowCount() > 0;
}

const TasksModel* Kid::getTasks() const
{
    return tasks_;
}

Theme* Kid::getTheme()
{
    return theme_;
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


void Kid::addTask(DueTask* task)
{
    connect(task, &DueTask::accomplished, this, [this, task]() { onTaskAccomplished(task); });
    task->setParent(this);
    tasks_->append(task);
}

void Kid::clearTasks()
{
    tasks_->clear();
}

void Kid::onTaskAccomplished(DueTask* task)
{
    tasks_->remove(task);
}
