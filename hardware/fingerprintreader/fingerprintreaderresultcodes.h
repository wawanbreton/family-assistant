#pragma once

#include <QtGlobal>

enum class FingerprintReaderResultCodes : quint8
{
    Success = 0,
    Fail = 1,
    Full = 4,
    NoUser = 5,
    UserExists = 6,
    Timeout = 8
};
