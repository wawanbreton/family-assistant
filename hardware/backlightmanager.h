#pragma once

#include <QObject>
#include <QPointer>
#include <QTime>
#include <QTimer>
#include <QVariantAnimation>

class BacklightManager : public QObject
{
    Q_OBJECT

public:
    explicit BacklightManager(QObject* parent = nullptr);

    virtual bool eventFilter(QObject* receiver, QEvent* event);

signals:
    void setBacklight(qreal power_percent);

private:
    enum class State
    {
        Asleep,
        WakingUp,
        Awake
    };

private:
    void onTimeoutAwake();

    void onTimeoutDay();

    void updateTargetBacklight();

private:
    State state_{ State::Awake };
    QTimer* const timer_awake_;
    QTimer* const timer_day_;
    int backlight_value_percent_{ -1 };
    QPointer<QVariantAnimation> backlight_animation_;
    QTime day_start_;
    QTime day_end_;
};
