#pragma once

#include <easyqt/enum.h>

class ThemeCategory : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

public:
    enum Enum
    {
        Point,
        PointsStorage,
    };

    ENUM_DEBUG()
    ENUM_GETALLVALUES()
};

Q_DECLARE_METATYPE(ThemeCategory::Enum)
