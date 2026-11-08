#pragma once

#include <QObject>
#include <QPointer>
#include <QTime>
#include <QTimer>
#include <QVariantAnimation>

#include "hardware/backlightstate.h"

class BacklightManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BacklightState::Enum backlightState MEMBER state_ NOTIFY backlightStateChanged)

public:
    explicit BacklightManager(QObject* parent = nullptr);

    virtual bool eventFilter(QObject* receiver, QEvent* event);

signals:
    void backlightStateChanged(BacklightState::Enum state);

    void setBacklight(qreal power_percent);

private:
    void setBacklightState(const BacklightState::Enum state);

    void onTimeoutAwake();

    void onTimeoutDay();

    void updateTargetBacklight();

private:
    BacklightState::Enum state_{ BacklightState::Awake };
    QTimer* const timer_awake_;
    QTimer* const timer_day_;
    int backlight_value_percent_{ -1 };
    QPointer<QVariantAnimation> backlight_animation_;
    QTime day_start_;
    QTime day_end_;
};
