#pragma once

#include "hardware/hardware.h"

class PhysicalHardware : public Hardware
{
    Q_OBJECT

public:
    explicit PhysicalHardware(QObject* parent = nullptr);
};
