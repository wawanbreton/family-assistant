#pragma once

#include <QObject>
#include <QQmlListProperty>

class Task;

class Kid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QQmlListProperty<Task> tasks READ getTasks NOTIFY tasksChanged)

public:
    explicit Kid(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getName() const;

    void setName(const QString& name);

    QQmlListProperty<Task> getTasks();

signals:
    void nameChanged(const QString& name);

    void tasksChanged();

private:
    QString name_;
    QList<Task*> tasks_;
};
