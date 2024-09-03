#include "kid.h"

#include <QJsonArray>
#include <QJsonObject>

#include "data/duetask.h"
#include "data/tasksmodel.h"
#include "data/theme.h"
#include "utils/json.h"


Kid::Kid(QObject* parent)
    : QObject{ parent }
    , tasks_(new TasksModel(this))
    , theme_(new Theme(this))
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
            auto task = new DueTask(this);
            task->load(task_object.toObject());
            addTask(task);
        }
    }

    iterator = json_object.constFind("theme");
    if (iterator != json_object.constEnd())
    {
        theme_->load(iterator.value().toObject());
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

bool Kid::hasTasks() const
{
    return tasks_->rowCount() > 0;
}

TasksModel* Kid::getTasks()
{
    return tasks_;
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
    tasks_->append(task);
}

void Kid::onTaskAccomplished(DueTask* task)
{
    tasks_->remove(task);
}
