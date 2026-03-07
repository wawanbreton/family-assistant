#pragma once

#include <easyqt/enum.h>

class ScreenTimeState : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        Plenty,  // There is still plenty of screen time
        NotMuch, // There is still screen time, but not much
        Out,     // There is no more screen time
    };

    ENUM_DEBUG()
};

Q_DECLARE_METATYPE(ScreenTimeState::Enum)
