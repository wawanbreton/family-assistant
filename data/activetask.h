#pragma once

#include <QUuid>

#include "data/task.h"

class ActiveTask : public Task
{
    Q_OBJECT

public:
    const QUuid& getUuid() const;

    virtual void load(const QJsonObject& json_object) override;

    static ActiveTask* makeAndLoad(const QJsonObject& json_object, QObject* parent = nullptr);

protected:
    explicit ActiveTask(QObject* parent = nullptr);

    explicit ActiveTask(const ActiveTask& other);

private:
    QUuid uuid_;
};
