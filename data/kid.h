#pragma once

#include <QObject>
#include <QUuid>

#include "data/tasksmodel.h"
#include "data/theme.h"

class ActiveTask;
class DueTask;

class Kid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(const TasksModel* tasks READ getTasks CONSTANT)
    Q_PROPERTY(Theme* theme READ getTheme CONSTANT)
    Q_PROPERTY(quint32 points READ getPoints WRITE setPoints NOTIFY pointsChanged)

public:
    explicit Kid(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QUuid& getUuid() const;

    const QString& getName() const;

    void setName(const QString& name);

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
    void nameChanged(const QString& name);

    void pointsChanged(const quint32 points, const qint32 delta);

private:
    void onTaskAccomplished(DueTask* task);

private:
    QUuid uuid_;
    QString name_;
    TasksModel* const tasks_;
    Theme* const theme_;
    quint32 points_{ 0 };
};
