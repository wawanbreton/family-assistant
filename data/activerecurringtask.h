#pragma once

#include "data/activetask.h"
#include "data/taskoccurences.h"

class ActiveRecurringTask : public ActiveTask
{
    Q_OBJECT

public:
    explicit ActiveRecurringTask(QObject* parent = nullptr);

    explicit ActiveRecurringTask(const ActiveRecurringTask& other);

    void load(const QJsonObject& json_object) override;

    void save(QJsonObject& json_object) const override;

    const QList<TaskOccurences>& getOccurences() const;

private:
    QList<TaskOccurences> occurences_;
};
