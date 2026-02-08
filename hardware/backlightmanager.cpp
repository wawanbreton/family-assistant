#include "backlightmanager.h"

#include <QDebug>
#include <QEvent>

#include "data/preferenceentry.h"
#include "data/preferences.h"


BacklightManager::BacklightManager(QObject* parent)
    : QObject(parent)
    , state_(State::Awake)
    , timer_awake_(new QTimer(this))
    , timer_day_(new QTimer(this))
    , day_start_(7, 0, 0)
    , day_end_(20, 0, 0)
{
    Preferences::access()->registerPreference(PreferenceEntry::BacklightPowerDayOff, QMetaType::Int, 20);
    Preferences::access()->registerPreference(PreferenceEntry::BacklightPowerDayOn, QMetaType::Int, 100);
    Preferences::access()->registerPreference(PreferenceEntry::BacklightPowerNightOff, QMetaType::Int, 0);
    Preferences::access()->registerPreference(PreferenceEntry::BacklightPowerNightOn, QMetaType::Int, 40);
    Preferences::access()->registerPreference(PreferenceEntry::BacklightSleepDelay, QMetaType::Int, 90000);
    Preferences::access()->registerPreference(PreferenceEntry::BacklightWakeUpAnimationDuration, QMetaType::Int, 200);
    Preferences::access()->registerPreference(PreferenceEntry::BacklightSleepAnimationDuration, QMetaType::Int, 3000);

    timer_awake_->setSingleShot(true);
    timer_awake_->setInterval(Preferences::get()->getInt(PreferenceEntry::BacklightSleepDelay));
    connect(timer_awake_, &QTimer::timeout, this, &BacklightManager::onTimeoutAwake);
    timer_awake_->start();

    connect(timer_day_, &QTimer::timeout, this, &BacklightManager::onTimeoutDay);
    onTimeoutDay();
    timer_day_->start();

    QTimer::singleShot(0, this, [this] { updateTargetBacklight(); });
}

bool BacklightManager::eventFilter(QObject* receiver, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove
        || event->type() == QEvent::MouseButtonRelease)
    {
        switch (state_)
        {
            case State::Awake:
                timer_awake_->start();
                break;
            case State::Asleep:
            {
                if (event->type() == QEvent::MouseButtonPress)
                {
                    state_ = State::WakingUp;
                    updateTargetBacklight();
                }
                return true;
            }
            case State::WakingUp:
            {
                if (event->type() == QEvent::MouseButtonRelease)
                {
                    state_ = State::Awake;
                    timer_awake_->start();
                }
                return true;
            }
        }
    }

    return QObject::eventFilter(receiver, event);
}

void BacklightManager::onTimeoutAwake()
{
    if (state_ == State::Awake)
    {
        state_ = State::Asleep;
        updateTargetBacklight();
    }
}

void BacklightManager::onTimeoutDay()
{
    updateTargetBacklight();

    const auto now = QDateTime::currentDateTime();
    QDateTime next_trigger_time;
    if (now.time() < day_start_)
    {
        next_trigger_time = QDateTime(now.date(), day_start_);
    }
    else if (now.time() < day_end_)
    {
        next_trigger_time = QDateTime(now.date(), day_end_);
    }
    else
    {
        next_trigger_time = QDateTime(now.date().addDays(1), day_start_);
    }

    timer_day_->setInterval(now.msecsTo(next_trigger_time));
}

void BacklightManager::updateTargetBacklight()
{
    const bool target_backlight_on = state_ != State::Asleep;
    const auto now = QTime::currentTime();
    const bool is_day = now >= day_start_ && now < day_end_;

    PreferenceEntry::Enum entry;
    if (is_day)
    {
        entry = target_backlight_on ? PreferenceEntry::BacklightPowerDayOn : PreferenceEntry::BacklightPowerDayOff;
    }
    else
    {
        entry = target_backlight_on ? PreferenceEntry::BacklightPowerNightOn : PreferenceEntry::BacklightPowerNightOff;
    }

    const int target_backlight_value_percent = Preferences::get()->getInt(entry);

    if (target_backlight_value_percent != backlight_value_percent_)
    {
        QVariant startValue = static_cast<qreal>(backlight_value_percent_);
        if (backlight_animation_)
        {
            startValue = backlight_animation_->currentValue();
            backlight_animation_->deleteLater();
        }

        backlight_animation_ = new QVariantAnimation(this);
        backlight_animation_->setStartValue(startValue);
        backlight_animation_->setEndValue(static_cast<qreal>(target_backlight_value_percent));
        backlight_animation_->setDuration(Preferences::get()->getInt(
            target_backlight_on ? PreferenceEntry::BacklightWakeUpAnimationDuration
                                : PreferenceEntry::BacklightSleepAnimationDuration));
        backlight_animation_->setEasingCurve(target_backlight_on ? QEasingCurve::OutCubic : QEasingCurve::InQuad);
        connect(
            backlight_animation_.data(),
            &QVariantAnimation::valueChanged,
            this,
            [this](const QVariant& value) { emit setBacklight(value.toReal()); });
        backlight_animation_->start(QAbstractAnimation::DeleteWhenStopped);

        backlight_value_percent_ = target_backlight_value_percent;
    }
}
