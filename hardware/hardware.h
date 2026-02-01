#pragma once

#include <QObject>

#include "easyqt/singleton.h"

class FingerprintReaderInterface;

class Hardware : public QObject
{
    Q_OBJECT
    SINGLETON(Hardware)

signals:
    void parentUnlock();

protected:
    virtual void setBacklight(const qreal power_percent) = 0;

private:
    FingerprintReaderInterface* fingerprint_reader_{ nullptr };
};
