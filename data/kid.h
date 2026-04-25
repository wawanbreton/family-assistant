#pragma once

#include <QObject>
#include <QUuid>

#include "data/screentimestate.h"
#include "data/tasksmodel.h"
#include "data/theme.h"
#include "data/user.h"

class QTimer;

class ActiveTask;
class DueTask;

class Kid : public User
{
    Q_OBJECT
    Q_PROPERTY(const TasksModel* tasks READ getTasks CONSTANT STORED false)
    Q_PROPERTY(Theme* theme READ getTheme CONSTANT STORED false)
    Q_PROPERTY(quint32 points READ getPoints WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(int64_t screen_time READ getScreenTime WRITE setScreenTime NOTIFY screenTimeChanged)
    Q_PROPERTY(QString screen_time_str READ getScreenTimeStr NOTIFY screenTimeChanged)
    Q_PROPERTY(bool screen_time_active READ isScreenTimeActive NOTIFY screenTimeActiveChanged)
    Q_PROPERTY(
        int64_t active_screen_time_duration READ getActiveScreenTimeDuration NOTIFY activeScreenTimeDurationChanged)
    Q_PROPERTY(QString active_screen_time_duration_str READ getActiveScreenTimeDurationStr NOTIFY
                   activeScreenTimeDurationChanged)
    Q_PROPERTY(ScreenTimeState::Enum screen_time_state READ getScreenTimeState NOTIFY screenTimeStateChanged)

public:
    explicit Kid(QObject* parent = nullptr);

    virtual void load(const QJsonObject& json_object) override;

    virtual void save(QJsonObject& object) const override;

    virtual bool isAdmin() const override;

    bool hasTasks() const;

    const TasksModel* getTasks() const;

    /*!
     * \brief Adds a task to be accomplished
     * \note Ownership of the given task is taken
     */
    void addTask(DueTask* task);

    void clearTasks();

    Theme* getTheme();

    quint32 getPoints() const;

    void setPoints(const quint32 points);

    int64_t getScreenTime() const;

    void setScreenTime(int64_t screen_time);

    QString getScreenTimeStr() const;

    Q_INVOKABLE void startScreenTime();

    Q_INVOKABLE void stopScreenTime();

    void resetScreenTime(const std::chrono::seconds screen_time);

    bool isScreenTimeActive() const;

    int64_t getActiveScreenTimeDuration() const;

    QString getActiveScreenTimeDurationStr() const;

    ScreenTimeState::Enum getScreenTimeState() const;

signals:
    void pointsChanged(const quint32 points, const qint32 delta);

    void screenTimeChanged();

    void screenTimeActiveChanged();

    void activeScreenTimeDurationChanged();

    void screenTimeStateChanged();

private:
    void decreaseScreenTime();

    static QString screenTimeDurationToString(const std::chrono::seconds duration);

private:
    TasksModel* const tasks_;
    Theme* const theme_;
    quint32 points_{ 0 };
    std::chrono::seconds screen_time_{};
    std::chrono::seconds active_screen_time_duration_{};
    std::chrono::seconds screen_time_penalty_count_{};
    QTimer* const timer_screen_time_;
};
