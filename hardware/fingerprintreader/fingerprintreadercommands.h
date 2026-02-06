#pragma once

#include <easyqt/enum.h>

class FingerprintReaderCommands : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum : quint8
    {
        AddFingerprint1 = 0x01,
        AddFingerprint2 = 0x02,
        AddFingerprint3 = 0x03,
        RemoveFingerprint = 0x04,
        RemoveAllFingerprints = 0x05,
        GetFingerprintsCount = 0x09,
        CheckUserFingerprint = 0x0b,
        CheckFingerprint = 0x0c,
        Sleep = 0x2c,
        SetReadFingerprintAddingMode = 0x2d,
    };

    ENUM_DEBUG()
};
