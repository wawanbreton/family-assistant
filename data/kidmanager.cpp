#include "kidmanager.h"

#include <QJsonArray>
#include <QJsonObject>

#include "data/kid.h"


SINGLETON_IMPL(KidManager)
SINGLETON_DESTRUCTOR_IMPL(KidManager)

KidManager::KidManager(QObject* parent)
    : QObject{ parent }
{
}

void KidManager::load(const QJsonObject& object)
{
    auto iterator = object.constFind("kids");
    if (iterator != object.constEnd())
    {
        QJsonArray kids_array = iterator.value().toArray();
        for (const QJsonValue& kid_object : kids_array)
        {
            auto kid = new Kid(this);
            kid->load(kid_object.toObject());
            kids_ << kid;
        }
    }
    else
    {
        qWarning() << "No kids defined in data";
    }
}

QQmlListProperty<Kid> KidManager::getKidsQmlList()
{
    return QQmlListProperty(this, &kids_);
}

const QList<Kid*>& KidManager::getKids()
{
    return kids_;
}
