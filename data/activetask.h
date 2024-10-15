#pragma once

#include <QUuid>

#include "data/task.h"
#include "data/taskoccurence.h"

class ActiveTask : public Task
{
    Q_OBJECT

public:
    explicit ActiveTask(QObject* parent = nullptr);

    explicit ActiveTask(const ActiveTask& other);

    const QUuid& getUuid() const;

    virtual void load(const QJsonObject& json_object);

    const QList<TaskOccurence>& getOccurences() const;

    bool isCasual() const;

private:
    QUuid uuid_;
    QList<TaskOccurence> occurences_;
};
