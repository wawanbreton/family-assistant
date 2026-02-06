#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <easyqt/singleton.h>

class Kid;
class User;

class UserManager : public QObject
{
    Q_OBJECT
    SINGLETON(UserManager)
    Q_PROPERTY(QList<Kid*> kids READ getKids CONSTANT)
    Q_PROPERTY(QList<User*> users READ getUsers CONSTANT)

public:
    void load(const QJsonObject& json_object);

    void save(QJsonObject& object) const;

    QList<Kid*> getKids() const;

    const QList<User*>& getUsers() const;

    Kid* find(const QUuid& uuid);

    Q_INVOKABLE int findIndex(const Kid* kid) const;

signals:
    void changed();

private:
    QList<User*> users_;
};
