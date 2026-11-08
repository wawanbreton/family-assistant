#pragma once

#include <QObject>

class AudioState : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

    public:
        enum Enum
        {
            None,
            Music,
            Bluetooth
        };
};
