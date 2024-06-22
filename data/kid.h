#pragma once

#include <QObject>
#include <QQmlListProperty>

class Task;

class Kid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QObjectList tasks READ getTasks NOTIFY tasksChanged)

public:
    explicit Kid(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getName() const;

    void setName(const QString& name);

    const QObjectList getTasks() const;

signals:
    void nameChanged(const QString& name);

    void tasksChanged();

private:
    void addTask(Task* task);

    void onTaskAccomplished(Task* task);

private:
    QString name_;
    QList<Task*> tasks_;
};
