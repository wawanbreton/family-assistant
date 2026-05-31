#include "task.h"

#include <QFileInfo>
#include <easyqt/datastorage.h>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <ranges>


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
    , announcements_(other.announcements_)
{
}

void Task::copyFrom(const Task* other)
{
    desc_ = other->desc_;
    icon_path_ = other->icon_path_;
    reward_ = other->reward_;
    announcements_ = other->announcements_;
}

void Task::load(const QJsonObject& json_object)
{
    desc_ = easyqt::Json::loadProperty(json_object, "desc", __METHOD__, desc_);
    setIcon(easyqt::Json::loadProperty(json_object, "icon", __METHOD__, QString()));
    reward_ = easyqt::Json::loadProperty(json_object, "reward", __METHOD__, reward_);

    std::ranges::transform(
        easyqt::Json::loadPropertyArray<int, QList>(
            json_object,
            "announcements",
            __METHOD__,
            easyqt::Json::WarnIfNotFound::No),
        std::back_inserter(announcements_),
        [](int announcement) { return std::chrono::minutes(announcement); });
}

void Task::save(QJsonObject& json_object) const
{
    json_object["desc"] = easyqt::Json::saveValue(desc_);
    json_object["icon"] = easyqt::Json::saveValue(getIcon());
    json_object["reward"] = easyqt::Json::saveValue(reward_);
    if (! announcements_.empty())
    {
        json_object["announcements"] = easyqt::Json::saveArray(
            announcements_
            | std::views::transform([](std::chrono::minutes announcement) { return announcement.count(); }));
    }
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

const QList<std::chrono::minutes>& Task::getAnnouncements() const
{
    return announcements_;
}
