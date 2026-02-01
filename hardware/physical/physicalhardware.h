#pragma once

#include "hardware/hardware.h"

class PhysicalHardware : public Hardware
{
    Q_OBJECT

public:
    explicit PhysicalHardware(QObject* parent = nullptr);

protected:
    virtual void setBacklight(const qreal power_percent) override;
};
