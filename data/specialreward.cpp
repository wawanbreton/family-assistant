#include "specialreward.h"

#include <easyqt/datastorage.h>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/resourcetype.h>


SpecialReward::SpecialReward(QObject* parent)
    : QObject{ parent }
{
}

void SpecialReward::load(const QJsonObject& json_object)
{
    name_ = easyqt::Json::loadProperty(json_object, "name", __METHOD__, name_);
    pronounced_name_ = easyqt::Json::loadProperty(
        json_object,
        "pronounced_name",
        __METHOD__,
        name_,
        easyqt::Json::WarnIfNotFound::No);
    cost_ = easyqt::Json::loadProperty(json_object, "cost", __METHOD__, cost_);

    const QString raw_icon = easyqt::Json::loadProperty(json_object, "icon", __METHOD__, icon_);
    icon_ = easyqt::DataStorage::get()->resourceFile(raw_icon, easyqt::ResourceType::Icon);
}

const QString& SpecialReward::getName() const
{
    return name_;
}

const QString& SpecialReward::getPronouncedName() const
{
    return pronounced_name_;
}

int SpecialReward::getCost() const
{
    return cost_;
}

const QString& SpecialReward::getIcon() const
{
    return icon_;
}
