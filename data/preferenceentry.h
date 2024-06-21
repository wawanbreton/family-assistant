#pragma once

#include <easyqt/enum.h>


class PreferenceEntry : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        TaskSoonDelay,       // The delay before a task due timestamp to consider that it should be in progress soon, in
                             // minutes
        TaskInProgressDelay, // The delay before a task due timestamp to consider that it should be in progress by now,
                             // in minutes
        TaskCloseToEndDelay, // The delay before a task due timestamp to consider that it should be done very soon, in
                             // minutes
    };

    ENUM_TOSTRING()
    ENUM_DEBUG()
};
