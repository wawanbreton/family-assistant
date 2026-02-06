#pragma once

#include <QObject>
#include <QUuid>

#include "data/tasksmodel.h"
#include "data/theme.h"
#include "data/user.h"

class ActiveTask;
class DueTask;

class Kid : public User
{
    Q_OBJECT
    Q_PROPERTY(const TasksModel* tasks READ getTasks CONSTANT STORED false)
    Q_PROPERTY(Theme* theme READ getTheme CONSTANT STORED false)
    Q_PROPERTY(quint32 points READ getPoints WRITE setPoints NOTIFY pointsChanged)

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

signals:
    void pointsChanged(const quint32 points, const qint32 delta);

private:
    TasksModel* const tasks_;
    Theme* const theme_;
    quint32 points_{ 0 };
};
