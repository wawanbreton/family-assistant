#include "activecasualtask.h"

#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "kid.h"
#include "kidmanager.h"


ActiveCasualTask::ActiveCasualTask(QObject* parent)
    : ActiveTask{ parent }
{
}

ActiveCasualTask::ActiveCasualTask(const ActiveCasualTask& other)
    : ActiveTask(other)
{
}


void ActiveCasualTask::load(const QJsonObject& json_object)
{
    ActiveTask::load(json_object);

    affected_kid_ = easyqt::Json::loadProperty(json_object, "affected_kid_uuid", __METHOD__, QUuid());
}

void ActiveCasualTask::save(QJsonObject& json_object) const
{
    ActiveTask::save(json_object);

    json_object["affected_kid_uuid"] = easyqt::Json::saveValue(affected_kid_.getUuid());
}

Kid* ActiveCasualTask::getAffectedKid() const
{
    return affected_kid_;
}

void ActiveCasualTask::setAffectedKid(Kid* affected_kid)
{
    if (affected_kid != affected_kid_)
    {
        affected_kid_ = affected_kid;
        emit affectedKidChanged();
    }
}
