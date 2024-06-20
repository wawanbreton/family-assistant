#pragma once

#include <QObject>
#include <QQmlListProperty>

class Kid;

class KidManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Kid> kids READ getKids NOTIFY kidsChanged)

public:
    explicit KidManager(QObject* parent = nullptr);

    void load(const QJsonObject& object);

    QQmlListProperty<Kid> getKids();

signals:
    void kidsChanged();

private:
    QList<Kid*> kids_;
};
