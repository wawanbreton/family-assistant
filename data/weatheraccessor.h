#pragma once

#include <QDateTime>
#include <QObject>
#include <QQmlListProperty>
#include <chrono>
#include <easyqt/singleton.h>

#include "data/constants.h"
#include "data/daymode.h"

using namespace std::chrono_literals;

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

class ForecastData;

class WeatherAccessor : public QObject
{
    Q_OBJECT
    SINGLETON(WeatherAccessor)
    Q_PROPERTY(QString temperature MEMBER _temperature NOTIFY currentDataChanged)
    Q_PROPERTY(QString icon MEMBER _icon NOTIFY currentDataChanged)
    Q_PROPERTY(QQmlListProperty<ForecastData> forecast READ getForecast NOTIFY forecastChanged)

public:
    QQmlListProperty<ForecastData> getForecast();

signals:
    void currentDataChanged();

    void forecastChanged();

    void dayModeChanged(DayMode::Enum mode);

private:
    void updateWeather();

    void updateDayMode();

    void onRequestCurrentFinished(QNetworkReply* reply);

    void onRequestForecastFinished(QNetworkReply* reply);

    QString getIcon(const QJsonValue& value) const;

    void setupRequest(QUrl& url);

private:
    QString _temperature{ Constants::empty };
    const QString _no_icon{};
    QString _icon{};
    QDateTime _lastGetSuccess;
    QList<ForecastData*> _forecast;
    QNetworkAccessManager* _network{ nullptr };
    DayMode::Enum _dayMode{ DayMode::Unknown };
    QDateTime _sunrise;
    QDateTime _sunset;
    QTimer* _timerUpdateDayMode{ nullptr };
    QTimer* const timer_update_weather_;
};


Q_DECLARE_METATYPE(QQmlListProperty<ForecastData>);
