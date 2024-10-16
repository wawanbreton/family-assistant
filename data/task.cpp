#include "task.h"

#include <easyqt/datastorage.h>
#include <easyqt/json.h>


using namespace std::chrono_literals;

Task::Task(QObject* parent)
    : QObject{ parent }
{
}

Task::Task(const Task& other)
    : QObject(other.parent())
    , desc_(other.desc_)
    , icon_path_(other.icon_path_)
    , reward_(other.reward_)
{
}

void Task::copyFrom(const Task* other)
{
    desc_ = other->desc_;
    icon_path_ = other->icon_path_;
    reward_ = other->reward_;
}

void Task::load(const QJsonObject& json_object)
{
    Json::mapValuesToObjectProperties(json_object, this);
}

const QString& Task::getDesc() const
{
    return desc_;
}

void Task::setDesc(const QString& desc)
{
    desc_ = desc;
}

const QString& Task::getIconPath() const
{
    return icon_path_;
}

void Task::setIcon(const QString& icon)
{
    icon_path_ = easyqt::DataStorage::findResource(icon, easyqt::ResourceType::Icon, { "png", "svg" });
}

quint32 Task::getReward() const
{
    return reward_;
}

void Task::setReward(quint32 reward)
{
    reward_ = reward;
}
