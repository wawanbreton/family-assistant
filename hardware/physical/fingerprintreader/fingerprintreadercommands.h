#pragma once

#include <easyqt/enum.h>

class FingerprintReaderCommands : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum : quint8
    {
        Sleep = 0x2c,
        SetReadFingerprintAddingMode = 0x2d,
    };

    ENUM_DEBUG()
};
