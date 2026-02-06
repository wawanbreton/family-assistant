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
    void onReadUsersCount();

    void onClearAllFingerprints();

    void onMatchUser();

    void onScanUser(int attempt, int user_id);

private:
    SimulatedHardwareWidget* const widget_;
};
