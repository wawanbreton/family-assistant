#pragma once

#include <QDateTime>
#include <QObject>
#include <QQmlEngine>

class ForecastData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int temperature READ getTemperature NOTIFY changed)
    Q_PROPERTY(QString temperatureStr READ getTemperatureStr NOTIFY changed)
    Q_PROPERTY(QString hourStr READ getHourStr NOTIFY changed)
    Q_PROPERTY(QString icon READ getIcon NOTIFY changed)

public:
    explicit ForecastData(QObject* parent = nullptr);

    const int& getTemperature() const
    {
        return _temperature;
    }

    QString getTemperatureStr() const;

    QString getHourStr() const;

    const QString& getIcon() const
    {
        return _icon;
    }

    const QDateTime& getTimestamp() const
    {
        return _timestamp;
    }

    void set(const double& timestamp, const double& temperature, const QString& icon);

signals:
    void changed();

private:
    QDateTime _timestamp;
    int _temperature{ 0 };
    QString _icon;
};
