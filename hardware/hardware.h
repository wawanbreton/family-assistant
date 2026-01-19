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

private:
    FingerprintReaderInterface* fingerprint_reader_{ nullptr };
};
