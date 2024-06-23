#pragma once

#include <QObject>
#include <QQmlListProperty>

#include "data/tasksmodel.h"

class Task;
class TasksModel;

class Kid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(TasksModel* tasks READ getTasks CONSTANT)
    Q_PROPERTY(quint32 points READ getPoints WRITE setPoints NOTIFY pointsChanged)

public:
    explicit Kid(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getName() const;

    void setName(const QString& name);

    TasksModel* getTasks();

    quint32 getPoints() const;

    void setPoints(const quint32 points);

signals:
    void nameChanged(const QString& name);

    void pointsChanged(const quint32 points, const qint32 delta);

private:
    void addTask(Task* task);

    void onTaskAccomplished(Task* task);

private:
    QString name_;
    TasksModel* const tasks_;
    quint32 points_{ 0 };
};
