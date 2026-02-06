#pragma once

#include <QTime>

#include "data/dayofweek.h"

struct TaskOccurence
{
    DayOfWeek::Enum day;
    QTime time;
};
