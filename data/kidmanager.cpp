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
