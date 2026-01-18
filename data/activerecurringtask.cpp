#include "activerecurringtask.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/parser.h>


ActiveRecurringTask::ActiveRecurringTask(QObject* parent)
    : ActiveTask{ parent }
{
}

ActiveRecurringTask::ActiveRecurringTask(const ActiveRecurringTask& other)
    : ActiveTask(other)
    , occurences_(other.occurences_)
{
}


void ActiveRecurringTask::load(const QJsonObject& json_object)
{
    ActiveTask::load(json_object);

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

void ActiveRecurringTask::save(QJsonObject& json_object) const
{
    ActiveTask::save(json_object);

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

const QList<TaskOccurences>& ActiveRecurringTask::getOccurences() const
{
    return occurences_;
}
