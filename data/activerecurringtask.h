#pragma once

#include "data/activetask.h"
#include "data/taskoccurence.h"

class ActiveRecurringTask : public ActiveTask
{
    Q_OBJECT

public:
    explicit ActiveRecurringTask(QObject* parent = nullptr);

    explicit ActiveRecurringTask(const ActiveRecurringTask& other);

    virtual void load(const QJsonObject& json_object) override;

    const QList<TaskOccurence>& getOccurences() const;

private:
    QList<TaskOccurence> occurences_;
};
