#include "simulatedhardware.h"

#include <QFile>
#include <QSerialPort>

#include "hardware/fingerprintreader/fingerprintreaderinterface.h"
#include "hardware/simulated/simulatedhardwarewidget.h"


SimulatedHardware::SimulatedHardware(QObject* parent)
    : Hardware{ parent }
    , widget_(new SimulatedHardwareWidget())
{
    widget_->move(0, 0);
    widget_->show();

    if (getFingerprint())
    {
        connect(widget_, &SimulatedHardwareWidget::readUsersCount, this, &SimulatedHardware::onReadUsersCount);
        connect(
            widget_,
            &SimulatedHardwareWidget::clearAllFingerprints,
            this,
            &SimulatedHardware::onClearAllFingerprints);
        connect(widget_, &SimulatedHardwareWidget::matchAny, this, &SimulatedHardware::onMatchAny);
        connect(widget_, &SimulatedHardwareWidget::matchUser, this, &SimulatedHardware::onMatchUser);
        connect(widget_, &SimulatedHardwareWidget::scanUser, this, &SimulatedHardware::onScanUser);
    }

    widget_->setFingerprintReaderEnabled(getFingerprint());
}

void SimulatedHardware::setBacklight(const qreal power_percent)
{
    widget_->setBacklight(power_percent);
}

void SimulatedHardware::onReadUsersCount()
{
    getFingerprint()->getFingerprintsCount(
        this,
        [this](quint16 count) { widget_->setUsersCount(count); },
        [this] { widget_->setUsersCount(std::nullopt); });
}

void SimulatedHardware::onClearAllFingerprints()
{
    getFingerprint()->removeAllFingerprints();
}

void SimulatedHardware::onMatchAny()
{
    getFingerprint()->checkFingerprint(
        10000,
        this,
        [this](quint16 user, quint8) { widget_->setMatchedAny(user); },
        [this] { widget_->setMatchedAny(std::nullopt); });
}

void SimulatedHardware::onMatchUser(int user_id)
{
    getFingerprint()->checkFingerprint(
        user_id,
        10000,
        this,
        [this] { widget_->setMatchedUser(true); },
        [this] { widget_->setMatchedUser(false); });
}

void SimulatedHardware::onScanUser(int attempt, int user_id)
{
    getFingerprint()->addFingerprint(
        user_id,
        1,
        attempt,
        10000,
        this,
        [this] { widget_->setScanAttemptSuccess(true); },
        [this] { widget_->setScanAttemptSuccess(false); });
}
