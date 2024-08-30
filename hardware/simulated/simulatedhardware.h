#pragma once

#include "hardware/hardware.h"

class SimulatedHardwareWidget;

class SimulatedHardware : public Hardware
{
    Q_OBJECT

public:
    explicit SimulatedHardware(QObject* parent = nullptr);

private:
    SimulatedHardwareWidget* const widget_;
};
