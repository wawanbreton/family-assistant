#pragma once

#include <easyqt/enum.h>

class BacklightState : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        Asleep,
        WakingUp,
        Awake
    };

    ENUM_DEBUG()
};

Q_DECLARE_METATYPE(BacklightState::Enum)
