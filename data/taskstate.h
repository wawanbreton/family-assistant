#pragma once

#include <easyqt/enum.h>

class TaskState : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        Early,      // Task is not to be accomplished any time soon
        Soon,       // Task is to be accomplished quite soon
        InProgress, // Task is supposed to be in progress
        CloseToEnd, // Task due time is approaching
        Late        // Task should have been accomplished by now
    };

    ENUM_DEBUG()
};

Q_DECLARE_METATYPE(TaskState::Enum)
