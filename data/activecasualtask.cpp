#include "activecasualtask.h"

#include <easyqt/debug.h>
#include <easyqt/json.h>

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

    affected_kid_ = Json::loadValue(json_object, "affected_kid_uuid", __METHOD__, QUuid());
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
