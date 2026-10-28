#include "forecastdata.h"

#include <QDebug>


ForecastData::ForecastData(QObject* parent)
    : QObject(parent)
{
}

QString ForecastData::getTemperatureStr() const
{
    return QString("%1°").arg(_temperature);
}

QString ForecastData::getHourStr() const
{
    return _timestamp.toLocalTime().toString("hh'h'");
}

void ForecastData::set(const double& timestamp, const double& temperature, const QString& icon)
{
    _timestamp = QDateTime::fromSecsSinceEpoch(qRound(timestamp), Qt::UTC);
    _temperature = qRound(temperature);
    _icon = icon;

    emit changed();
}
