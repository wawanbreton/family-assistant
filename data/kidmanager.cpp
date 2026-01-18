#include "kidmanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>

#include "data/kid.h"


SINGLETON_IMPL(KidManager)
SINGLETON_DESTRUCTOR_IMPL(KidManager)

KidManager::KidManager(QObject* parent)
    : QObject{ parent }
{
}

void KidManager::load(const QJsonObject& json_object)
{
    QList<QJsonObject> kids_objects
        = easyqt::Json::loadPropertyArray<QJsonObject, QList>(json_object, "kids", __METHOD__);
    for (const QJsonObject& kid_object : kids_objects)
    {
        auto kid = new Kid(this);
        kid->load(kid_object);
        connect(kid, &Kid::changed, this, &KidManager::changed);
        kids_ << kid;
    }
}

void KidManager::save(QJsonObject& object) const
{
    QJsonArray kids_array;
    for (const Kid* kid : kids_)
    {
        QJsonObject kid_object;
        kid->save(kid_object);
        kids_array.append(kid_object);
    }
    object["kids"] = kids_array;
}

QQmlListProperty<Kid> KidManager::getKidsQmlList()
{
    return QQmlListProperty(this, &kids_);
}

const QList<Kid*>& KidManager::getKids()
{
    return kids_;
}

Kid* KidManager::find(const QUuid& uuid)
{
    auto iterator
        = std::find_if(kids_.begin(), kids_.end(), [&uuid](const Kid* kid) { return kid->getUuid() == uuid; });
    if (iterator != kids_.end())
    {
        return *iterator;
    }

    return nullptr;
}

int KidManager::findIndex(const Kid* kid) const
{
    return kids_.indexOf(kid);
}
