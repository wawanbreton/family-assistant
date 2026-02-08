#pragma once

#include <QUuid>
#include <easyqt/lazypointer.h>

#include "data/kid.h"
#include "data/task.h"
#include "data/taskoccurences.h"

Q_MOC_INCLUDE("data/kid.h")

class Kid;
class UserManager;

class ActiveTask : public Task
{
    Q_OBJECT
    Q_PROPERTY(Kid* affected_kid READ getAffectedKid WRITE setAffectedKid NOTIFY affectedKidChanged)

public:
    explicit ActiveTask(QObject* parent = nullptr);

    const QUuid& getUuid() const;

    void load(const QJsonObject& json_object) override;

    void save(QJsonObject& json_object) const override;

    const QList<TaskOccurences>& getOccurences() const;

    Kid* getAffectedKid() const;

    void setAffectedKid(Kid* affected_kid);

    std::optional<int> getActivationDelay() const;

signals:
    void affectedKidChanged();

private:
    QUuid uuid_;
    easyqt::LazyPointer<Kid, UserManager> affected_kid_;
    QList<TaskOccurences> occurences_;
    std::optional<int> activation_delay_;
};
