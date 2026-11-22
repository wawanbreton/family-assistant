#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <easyqt/singleton.h>

class SpecialReward;

class SpecialRewardsModel : public QObject
{
    SINGLETON(SpecialRewardsModel)
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SpecialReward> special_rewards READ getSpecialRewards CONSTANT)

public:
    void load(const QJsonObject& json_object);

public:
    QQmlListProperty<SpecialReward> getSpecialRewards();

private:
    QList<SpecialReward*> special_rewards_;
};
