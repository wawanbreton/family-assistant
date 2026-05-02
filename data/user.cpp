#include "user.h"

#include <QJsonArray>
#include <QJsonObject>
#include <easyqt/debug.h>
#include <easyqt/json.h>
#include <easyqt/parser.h>


User::User(QObject* parent)
    : QObject{ parent }
    , uuid_(QUuid::createUuid())
{
    connect(this, &User::nameChanged, this, &User::changed);
    connect(this, &User::fingerprintIdChanged, this, &User::changed);
}

void User::load(const QJsonObject& json_object)
{
    uuid_ = easyqt::Json::loadProperty(json_object, "uuid", __METHOD__, uuid_);
    name_ = easyqt::Json::loadProperty(json_object, "name", __METHOD__, name_);
    pronounced_name_ = easyqt::Json::loadProperty(
        json_object,
        "pronounced_name",
        __METHOD__,
        pronounced_name_,
        easyqt::Json::WarnIfNotFound::No);
    fingerprint_id_
        = easyqt::Json::loadProperty<int>(json_object, "fingerprint", __METHOD__, easyqt::Json::WarnIfNotFound::No);
}

void User::save(QJsonObject& object) const
{
    object["uuid"] = easyqt::Json::saveValue(uuid_);
    object["name"] = easyqt::Json::saveValue(name_);
    if (! pronounced_name_.isEmpty())
    {
        object["pronounced_name"] = easyqt::Json::saveValue(pronounced_name_);
    }
    if (fingerprint_id_.has_value())
    {
        object["fingerprint"] = easyqt::Json::saveValue(*fingerprint_id_);
    }
}

const QUuid& User::getUuid() const
{
    return uuid_;
}

const QString& User::getName() const
{
    return name_;
}

void User::setName(const QString& name)
{
    if (name != name_)
    {
        name_ = name;
        emit nameChanged(name_);
    }
}

const QString& User::getPronouncedName() const
{
    return pronounced_name_.isEmpty() ? name_ : pronounced_name_;
}

std::optional<int> User::getFingerprintId() const
{
    return fingerprint_id_;
}

void User::setFingerprintId(int fingerprint_id)
{
    if (fingerprint_id != fingerprint_id_)
    {
        fingerprint_id_ = fingerprint_id;
        emit fingerprintIdChanged(*fingerprint_id_);
    }
}

bool User::isAdmin() const
{
    return true;
}
