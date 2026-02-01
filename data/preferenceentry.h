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
        CasualTaskDuration,  // The time to accomplish a casual start one it has been addded, in minutes
        BacklightPowerDayOn, // Backlight power when turned on, during the day [0-100]
        BacklightPowerDayOff,            // Backlight power when turned off, during the day [0-100]
        BacklightPowerNightOn,           // Backlight power when turned on, during the day [0-100]
        BacklightPowerNightOff,          // Backlight power when turned off, during the day [0-100]
        BacklightSleepDelay,             // Inactivity delay before screen goes into low-backlight mode, in milliseconds
        BacklightSleepAnimationDuration, // In milliseconds
        BacklightWakeUpAnimationDuration, // In milliseconds
    };

    ENUM_TOSTRING()
    ENUM_DEBUG()
};
