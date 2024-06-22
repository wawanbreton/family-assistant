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

public:
    explicit Kid(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getName() const;

    void setName(const QString& name);

    TasksModel* getTasks();

signals:
    void nameChanged(const QString& name);

private:
    void addTask(Task* task);

    void onTaskAccomplished(Task* task);

private:
    QString name_;
    TasksModel* const tasks_;
};
