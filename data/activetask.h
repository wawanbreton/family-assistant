#pragma once

#include "data/task.h"
#include "data/taskoccurence.h"

class ActiveTask : public Task
{
    Q_OBJECT

public:
    explicit ActiveTask(QObject* parent = nullptr);

    virtual void load(const QJsonObject& json_object);

    bool isRegular() const;

private:
    QList<TaskOccurence> occurences_;
};
