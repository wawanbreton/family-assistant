#pragma once

#include "easyqt/enum.h"

class DayOfWeek : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        Monday = Qt::Monday,
        Tuesday = Qt::Tuesday,
        Wednesday = Qt::Wednesday,
        Thursday = Qt::Thursday,
        Friday = Qt::Friday,
        Saturday = Qt::Saturday,
        Sunday = Qt::Sunday,
    };

    ENUM_DEBUG()
    ENUM_TOSTRING()
    ENUM_FROMSTRING()
};
