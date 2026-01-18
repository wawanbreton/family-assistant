#include "task.h"

#include <QFileInfo>
#include <easyqt/datastorage.h>
#include <easyqt/debug.h>
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
    desc_ = easyqt::Json::loadProperty(json_object, "desc", __METHOD__, desc_);
    setIcon(easyqt::Json::loadProperty(json_object, "icon", __METHOD__, QString()));
    reward_ = easyqt::Json::loadProperty(json_object, "reward", __METHOD__, reward_);
}

void Task::save(QJsonObject& json_object) const
{
    json_object["desc"] = easyqt::Json::saveValue(desc_);
    json_object["icon"] = easyqt::Json::saveValue(getIcon());
    json_object["reward"] = easyqt::Json::saveValue(reward_);
}

const QString& Task::getDesc() const
{
    return desc_;
}

const QString& Task::getIconPath() const
{
    return icon_path_;
}

QString Task::getIcon() const
{
    if (icon_path_.isEmpty())
    {
        return "";
    }

    return QFileInfo(icon_path_).baseName();
}

void Task::setIcon(const QString& icon)
{
    icon_path_ = easyqt::DataStorage::findResource(icon, easyqt::ResourceType::Icon, { "png", "svg" });
}

quint32 Task::getReward() const
{
    return reward_;
}
