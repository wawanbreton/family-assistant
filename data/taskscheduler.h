#pragma once

#include <QObject>
#include <easyqt/singleton.h>


class TaskScheduler : public QObject
{
    Q_OBJECT
    SINGLETON(TaskScheduler)
};
