#pragma once

#include <QUuid>

#include "data/task.h"

class ActiveTask : public Task
{
    Q_OBJECT

public:
    const QUuid& getUuid() const;

    void load(const QJsonObject& json_object) override;

    void save(QJsonObject& json_object) const override;

    static ActiveTask* makeAndLoad(const QJsonObject& json_object, QObject* parent = nullptr);

protected:
    explicit ActiveTask(QObject* parent = nullptr);

    explicit ActiveTask(const ActiveTask& other);

private:
    QUuid uuid_;
};
