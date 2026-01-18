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
    connect(this, &Kid::nameChanged, this, &Kid::changed);
    connect(this, &Kid::pointsChanged, this, &Kid::changed);
    connect(tasks_, &TasksModel::changed, this, &Kid::changed);
    connect(theme_, &Theme::changed, this, &Kid::changed);
}

void Kid::load(const QJsonObject& json_object)
{
    uuid_ = easyqt::Json::loadProperty(json_object, "uuid", __METHOD__, uuid_);
    name_ = easyqt::Json::loadProperty(json_object, "name", __METHOD__, name_);
    points_ = easyqt::Json::loadProperty(json_object, "points", __METHOD__, points_);

    QList<QJsonObject> tasks_array
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "tasks", __METHOD__);
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

    std::optional<QJsonObject> theme_object = easyqt::Json::loadProperty<QJsonObject>(json_object, "theme", __METHOD__);
    if (theme_object.has_value())
    {
        theme_->load(*theme_object);
    }
}

void Kid::save(QJsonObject& object) const
{
    object["uuid"] = easyqt::Json::saveValue(uuid_);
    object["name"] = easyqt::Json::saveValue(name_);
    object["points"] = easyqt::Json::saveValue(points_);

    QJsonObject theme_object;
    theme_->save(theme_object);
    object["theme"] = theme_object;

    QJsonArray tasks_array;
    tasks_->save(tasks_array);
    object["tasks"] = tasks_array;
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
    tasks_->append(task);
}

void Kid::clearTasks()
{
    tasks_->clear();
}
