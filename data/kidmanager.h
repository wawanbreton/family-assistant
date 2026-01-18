#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <easyqt/singleton.h>

class Kid;

class KidManager : public QObject
{
    Q_OBJECT
    SINGLETON(KidManager)
    Q_PROPERTY(QQmlListProperty<Kid> kids READ getKidsQmlList CONSTANT)

public:
    void load(const QJsonObject& json_object);

    void save(QJsonObject& object) const;

    QQmlListProperty<Kid> getKidsQmlList();

    const QList<Kid*>& getKids();

    Kid* find(const QUuid& uuid);

    Q_INVOKABLE int findIndex(const Kid* kid) const;

signals:
    void changed();

private:
    QList<Kid*> kids_;
};
