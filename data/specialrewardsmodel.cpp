#include "specialrewardsmodel.h"

#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/specialreward.h"


SINGLETON_IMPL(SpecialRewardsModel)
SINGLETON_DESTRUCTOR_IMPL(SpecialRewardsModel)

SpecialRewardsModel::SpecialRewardsModel(QObject* parent)
    : QObject{ parent }
{
}

void SpecialRewardsModel::load(const QJsonObject& json_object)
{
    QList<QJsonObject> special_rewards_objects
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "special_rewards", __METHOD__);
    for (const QJsonObject& special_reward_object : special_rewards_objects)
    {
        auto special_reward = new SpecialReward(this);
        special_reward->load(special_reward_object);
        special_rewards_ << special_reward;
    }
}

QQmlListProperty<SpecialReward> SpecialRewardsModel::getSpecialRewards()
{
    return QQmlListProperty<SpecialReward>(this, &special_rewards_);
}