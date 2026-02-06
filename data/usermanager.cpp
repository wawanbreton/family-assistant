#include "usermanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/kid.h"


SINGLETON_IMPL(UserManager)
SINGLETON_DESTRUCTOR_IMPL(UserManager)

UserManager::UserManager(QObject* parent)
    : QObject{ parent }
{
}

void UserManager::load(const QJsonObject& json_object)
{
    QList<QJsonObject> users_objects
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "users", __METHOD__);
    for (const QJsonObject& user_object : users_objects)
    {
        User* user;
        if (user_object.contains("points"))
        {
            user = new Kid(this);
        }
        else
        {
            user = new User(this);
        }
        user->load(user_object);
        connect(user, &Kid::changed, this, &UserManager::changed);
        users_ << user;
    }
}

void UserManager::save(QJsonObject& object) const
{
    QJsonArray users_array;
    for (const User* user : users_)
    {
        QJsonObject user_object;
        user->save(user_object);
        users_array.append(user_object);
    }
    object["users"] = users_array;
}

QList<Kid*> UserManager::getKids() const
{
    QList<Kid*> kids;

    for (User* user : users_)
    {
        if (Kid* kid = qobject_cast<Kid*>(user))
        {
            kids << kid;
        }
    }

    return kids;
}

const QList<User*>& UserManager::getUsers() const
{
    return users_;
}

Kid* UserManager::find(const QUuid& uuid)
{
    auto iterator = std::ranges::find_if(users_, [&uuid](const User* kid) { return kid->getUuid() == uuid; });
    if (iterator != users_.end())
    {
        return qobject_cast<Kid*>(*iterator);
    }

    return nullptr;
}

int UserManager::findIndex(const Kid* kid) const
{
    return getKids().indexOf(kid);
}
