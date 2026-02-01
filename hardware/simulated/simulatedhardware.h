#pragma once

#include "hardware/hardware.h"

class SimulatedHardwareWidget;

class SimulatedHardware : public Hardware
{
    Q_OBJECT

public:
    explicit SimulatedHardware(QObject* parent = nullptr);

protected:
    virtual void setBacklight(const qreal power_percent) override;

private:
    SimulatedHardwareWidget* const widget_;
};
