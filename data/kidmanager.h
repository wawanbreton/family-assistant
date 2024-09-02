#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <easyqt/singleton.h>

class Kid;

class KidManager : public QObject
{
    Q_OBJECT
    SINGLETON(KidManager)
    Q_PROPERTY(QQmlListProperty<Kid> kids READ getKidsQmlList NOTIFY kidsChanged)

public:
    void load(const QJsonObject& object);

    QQmlListProperty<Kid> getKidsQmlList();

    const QList<Kid*>& getKids();

signals:
    void kidsChanged();

private:
    QList<Kid*> kids_;
};
