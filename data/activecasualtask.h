#pragma once

#include <easyqt/lazypointer.h>

#include "data/activetask.h"

Q_MOC_INCLUDE("data/kid.h")

class Kid;
class KidManager;

class ActiveCasualTask : public ActiveTask
{
    Q_OBJECT
    Q_PROPERTY(Kid* affected_kid READ getAffectedKid WRITE setAffectedKid NOTIFY affectedKidChanged)

public:
    explicit ActiveCasualTask(QObject* parent = nullptr);

    explicit ActiveCasualTask(const ActiveCasualTask& other);

    void load(const QJsonObject& json_object) override;

    void save(QJsonObject& json_object) const override;

    Kid* getAffectedKid() const;

    void setAffectedKid(Kid* affected_kid);

signals:
    void affectedKidChanged();

private:
    easyqt::LazyPointer<Kid, KidManager> affected_kid_;
};
