#include "activetask.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/taskoccurence.h"


ActiveTask::ActiveTask(QObject* parent)
    : Task{ parent }
{
}

ActiveTask::ActiveTask(const ActiveTask& other)
    : Task(other)
    , occurences_(other.occurences_)
{
}

const QUuid& ActiveTask::getUuid() const
{
    return uuid_;
}

void ActiveTask::load(const QJsonObject& json_object)
{
    Task::load(json_object);

    uuid_ = Json::loadValue(json_object, "uuid", __METHOD__, QUuid());

    auto iterator = json_object.constFind("due_times");
    if (iterator != json_object.constEnd())
    {
        QJsonArray due_times_array = iterator.value().toArray();
        for (const QJsonValue& due_time_value : due_times_array)
        {
            const QJsonObject due_time_object = due_time_value.toObject();

            QList<DayOfWeek::Enum> days;
            auto iterator_days = due_time_object.constFind("days");
            if (iterator_days != due_time_object.constEnd())
            {
                const QJsonArray days_array = iterator_days.value().toArray();
                for (const QJsonValue& day_value : days_array)
                {
                    std::optional<DayOfWeek::Enum> day = DayOfWeek::fromString(day_value.toString());
                    if (day.has_value())
                    {
                        days << day.value();
                    }
                }
            }

            QList<QTime> times;
            auto iterator_times = due_time_object.constFind("times");
            if (iterator_times != due_time_object.constEnd())
            {
                const QJsonArray times_array = iterator_times.value().toArray();
                for (const QJsonValue& time_value : times_array)
                {
                    auto time = QTime::fromString(time_value.toString(), "HH:mm");
                    if (time.isValid())
                    {
                        times << time;
                    }
                }
            }

            for (DayOfWeek::Enum day : days)
            {
                for (const QTime& time : times)
                {
                    occurences_ << TaskOccurence{ day, time };
                }
            }
        }
    }
}

const QList<TaskOccurence>& ActiveTask::getOccurences() const
{
    return occurences_;
}

bool ActiveTask::isCasual() const
{
    return occurences_.isEmpty();
}
