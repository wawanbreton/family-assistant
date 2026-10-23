#pragma once

#include <QObject>
#include <easyqt/singleton.h>

class TimeAccessor : public QObject
{
    Q_OBJECT
    SINGLETON(TimeAccessor)
    Q_PROPERTY(QString time MEMBER _time NOTIFY timeChanged)
    Q_PROPERTY(QString date MEMBER _date NOTIFY dateChanged)

signals:
    void timeChanged(const QString& time);

    void dateChanged(const QString& date);

private:
    void updateDateTime();

private:
    QString _time;
    QString _date;
};
