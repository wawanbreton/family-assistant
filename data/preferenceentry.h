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
        WeeklyScreenTime,                 // In minutes
        AnnouncementRepeats,              // When a vocal announcement is made, it is repeated N times
        AnnouncementFirstVolume,          // Volume of the first announcement, [0-1], default 1
        AnnouncementLastVolume,           // Volume of the last announcement, [0-1], default 1
        AnnouncementAudioDevice, // Name of the audio device the announcements should be made with, empty for default
        ScreenTimePenaltyDelay,  // When screen time is over, a penalty point is taken every <delay> seconds
        ScreenTimeOverPreannouncements, // When screen time is (almost) due, vocal announcements are made at these
                                        // times. This is a list of negative (or null) ints, in minutes.
        WeatherLatitude,                // Latitude of the position for weather forecast
        WeatherLongitude,               // Longitude of the position for weather forecast
        WeatherAPIKey,                  // OpenWeatherMap API key
        DlnaDeviceDescriptionUrl,       // URL of the DLNA media server device description
        DlnaMainMusicPath, // Path to the DLNA folder containing music data, or empty to browse all the folders
        MusicAudioDevice,  // Name of the audio device the music should be played with, empty for default
        MusicMinVolume,    // Minimum volume to play the music, [0-1], default 0
        MusicMaxVolume,    // Minimum volume to play the music, [0-1], default 1
    };

    ENUM_TOSTRING()
    ENUM_DEBUG()
};
