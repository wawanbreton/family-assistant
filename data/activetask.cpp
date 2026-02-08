#include "activetask.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/kid.h"
#include "data/usermanager.h"


ActiveTask::ActiveTask(QObject* parent)
    : Task{ parent }
{
}

const QUuid& ActiveTask::getUuid() const
{
    return uuid_;
}

void ActiveTask::load(const QJsonObject& json_object)
{
    Task::load(json_object);

    uuid_ = easyqt::Json::loadProperty(json_object, "uuid", __METHOD__, QUuid());
    affected_kid_ = easyqt::Json::loadProperty(
        json_object,
        "affected_kid_uuid",
        __METHOD__,
        QUuid(),
        easyqt::Json::WarnIfNotFound::No);

    activation_delay_ = easyqt::Json::loadProperty<int>(
        json_object,
        "activation_delay",
        __METHOD__,
        easyqt::Json::WarnIfNotFound::No);

    QList<QJsonObject> occurences
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "occurences", __METHOD__);
    for (const QJsonObject& occurences_object : occurences)
    {
        QSet<DayOfWeek::Enum> days
            = easyqt::Json::loadPropertyArrayEnum<DayOfWeek, QSet>(occurences_object, "days", __METHOD__);
        QSet<QTime> times = easyqt::Json::loadPropertyArray<QTime, QSet>(occurences_object, "times", __METHOD__);

        if (! days.isEmpty() && ! times.isEmpty())
        {
            occurences_ << TaskOccurences{ days, times };
        }
    }
}

void ActiveTask::save(QJsonObject& json_object) const
{
    Task::save(json_object);

    json_object["uuid"] = easyqt::Json::saveValue(uuid_);

    const QUuid affected_kid_uuid = affected_kid_.getUuid();
    if (! affected_kid_uuid.isNull())
    {
        json_object["affected_kid_uuid"] = easyqt::Json::saveValue(affected_kid_.getUuid());
    }

    if (activation_delay_.has_value())
    {
        json_object["activation_delay"] = easyqt::Json::saveValue(activation_delay_.value());
    }

    QJsonArray occurences_array;

    for (const TaskOccurences& occurences : occurences_)
    {
        QJsonObject occurences_objects;
        occurences_objects["days"] = easyqt::Json::saveArrayEnum<DayOfWeek>(occurences.days);
        occurences_objects["times"] = easyqt::Json::saveArray(occurences.times);
        occurences_array.append(occurences_objects);
    }

    json_object["occurences"] = occurences_array;
}

const QList<TaskOccurences>& ActiveTask::getOccurences() const
{
    return occurences_;
}

Kid* ActiveTask::getAffectedKid() const
{
    return affected_kid_;
}

void ActiveTask::setAffectedKid(Kid* affected_kid)
{
    if (affected_kid != affected_kid_)
    {
        affected_kid_ = affected_kid;
        emit affectedKidChanged();
    }
}

std::optional<int> ActiveTask::getActivationDelay() const
{
    return activation_delay_;
}
