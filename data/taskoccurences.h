#pragma once

#include <QTime>

#include "data/dayofweek.h"

struct TaskOccurences
{
    QSet<DayOfWeek::Enum> days;
    QSet<QTime> times;
};
