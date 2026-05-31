#include "kid.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/parser.h>

#include "data/announcementmanager.h"
#include "data/duetask.h"
#include "data/preferences.h"
#include "data/taskscheduler.h"
#include "data/tasksmodel.h"
#include "data/theme.h"


using namespace std::chrono_literals;

Kid::Kid(QObject* parent)
    : User{ parent }
    , tasks_(new TasksModel(this))
    , theme_(new Theme(this))
    , timer_screen_time_(new QTimer(this))
{
    connect(this, &Kid::pointsChanged, this, &Kid::changed);
    connect(tasks_, &TasksModel::changed, this, &Kid::changed);
    connect(theme_, &Theme::changed, this, &Kid::changed);

    timer_screen_time_->setInterval(1s);
    connect(timer_screen_time_, &QTimer::timeout, this, &Kid::decreaseScreenTime);
}

void Kid::load(const QJsonObject& json_object)
{
    User::load(json_object);

    points_ = easyqt::Json::loadProperty(json_object, "points", __METHOD__, points_);

    auto bla = easyqt::Json::loadProperty(json_object, "screen_time", __METHOD__, screen_time_.count());
    screen_time_ = std::chrono::seconds(
        easyqt::Json::loadProperty(json_object, "screen_time", __METHOD__, screen_time_.count()));

    QList<QJsonObject> tasks_array
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "tasks", __METHOD__);
    for (const QJsonValue& task_object : tasks_array)
    {
        auto task = new DueTask(this);
        if (task->load(task_object.toObject()))
        {
            task->updateState();
            addTask(task);
        }
        else
        {
            delete task;
        }
    }

    std::optional<QJsonObject> theme_object = easyqt::Json::loadProperty<QJsonObject>(json_object, "theme", __METHOD__);
    if (theme_object.has_value())
    {
        theme_->load(*theme_object);
    }
}

void Kid::save(QJsonObject& object) const
{
    User::save(object);

    object["points"] = easyqt::Json::saveValue(points_);
    object["screen_time"] = easyqt::Json::saveValue(screen_time_.count());

    QJsonObject theme_object;
    theme_->save(theme_object);
    object["theme"] = theme_object;

    QJsonArray tasks_array;
    tasks_->save(tasks_array);
    object["tasks"] = tasks_array;
}

bool Kid::isAdmin() const
{
    return false;
}

bool Kid::hasTasks() const
{
    return tasks_->rowCount() > 0;
}

const TasksModel* Kid::getTasks() const
{
    return tasks_;
}

Theme* Kid::getTheme()
{
    return theme_;
}

qint32 Kid::getPoints() const
{
    return points_;
}

void Kid::setPoints(const qint32 points)
{
    if (points != points_)
    {
        const qint32 delta = points - points_;
        points_ = points;
        emit pointsChanged(points_, delta);
    }
}

int64_t Kid::getScreenTime() const
{
    return screen_time_.count();
}

void Kid::setScreenTime(int64_t screen_time)
{
    const auto screen_time_sec = std::chrono::seconds(screen_time);
    if (screen_time_sec != screen_time_)
    {
        screen_time_ = screen_time_sec;
        emit screenTimeChanged();
        emit screenTimeStateChanged();
    }
}

QString Kid::getScreenTimeStr() const
{
    return screenTimeDurationToString(screen_time_);
}

void Kid::startScreenTime()
{
    active_screen_time_duration_ = 0s;
    timer_screen_time_->start();
    emit screenTimeActiveChanged();
    emit activeScreenTimeDurationChanged();
    emit changed();
}

void Kid::stopScreenTime()
{
    timer_screen_time_->stop();
    emit screenTimeActiveChanged();
    emit changed();
}

void Kid::resetScreenTime(const std::chrono::seconds screen_time)
{
    screen_time_ = screen_time;
    emit changed();
    emit screenTimeChanged();
    emit screenTimeStateChanged();
}

bool Kid::isScreenTimeActive() const
{
    return timer_screen_time_->isActive();
}

int64_t Kid::getActiveScreenTimeDuration() const
{
    return active_screen_time_duration_.count();
}

QString Kid::getActiveScreenTimeDurationStr() const
{
    return screenTimeDurationToString(active_screen_time_duration_);
}

ScreenTimeState::Enum Kid::getScreenTimeState() const
{
    if (screen_time_ >= 1h)
    {
        return ScreenTimeState::Plenty;
    }
    else if (screen_time_ > 0s)
    {
        return ScreenTimeState::NotMuch;
    }
    else
    {
        return ScreenTimeState::Out;
    }
}

void Kid::decreaseScreenTime()
{
    screen_time_ = screen_time_ - 1s;
    active_screen_time_duration_++;

    const bool full_minute = screen_time_.count() % 60 == 0;
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(screen_time_);

    emit screenTimeChanged();
    emit activeScreenTimeDurationChanged();
    emit screenTimeStateChanged();

    if (screen_time_ < 0s)
    {
        const int penalty_delay = Preferences::get()->getInt(PreferenceEntry::ScreenTimePenaltyDelay);
        if (std::abs(screen_time_.count()) % penalty_delay == 0)
        {
            setPoints(static_cast<int>(getPoints()) - 1);
        }
    }

    if (full_minute)
    {
        int announcement_minutes = -minutes.count();
        const QList<int> announcements_times
            = Preferences::get()->getIntList(PreferenceEntry::ScreenTimeOverPreannouncements);
        if (announcements_times.contains(announcement_minutes))
        {
            QString message;
            if (announcement_minutes == 0)
            {
                message = "Le temps d'écran de %1 est terminé";
            }
            else if (announcement_minutes < 0)
            {
                message = QString("Le temps d'écran de %2 sera terminé dans %1 minutes").arg(-(announcement_minutes));
            }
            else
            {
                message = QString("Le temps d'écran de %2 est terminé depuis %1 minutes").arg(announcement_minutes);
            }

            AnnouncementManager::access()->addAnnouncement(message.arg(getPronouncedName()));
        }

        // Count every seconds, but only save every minute to avoid killing the SSD :)
        emit changed();
    }
}

QString Kid::screenTimeDurationToString(const std::chrono::seconds duration)
{
    const std::chrono::milliseconds duration_ms = duration > 0s ? duration : 0s;
    return QTime::fromMSecsSinceStartOfDay(duration_ms.count()).toString("HH:mm");
}

void Kid::addTask(DueTask* task)
{
    tasks_->append(task);
}

void Kid::clearTasks()
{
    tasks_->clear();
}
