#pragma once

#include <QObject>

#include "easyqt/singleton.h"

class Hardware : public QObject
{
    Q_OBJECT
    SINGLETON(Hardware)

signals:
    void parentUnlock();
};
