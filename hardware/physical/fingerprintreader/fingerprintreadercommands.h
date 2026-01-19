#pragma once

#include <easyqt/enum.h>

class FingerprintReaderCommands : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        Sleep = 0x2c,
    };

    ENUM_DEBUG()
};
