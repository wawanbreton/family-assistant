#include "weatheraccessor.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QUrlQuery>
#include <easyqt/datastorage.h>

#include "constants.h"
#include "data/forecastdata.h"
#include "data/preferences.h"


SINGLETON_IMPL(WeatherAccessor)
SINGLETON_DESTRUCTOR_IMPL(WeatherAccessor)

WeatherAccessor::WeatherAccessor(QObject* parent)
    : QObject(parent)
    , _no_icon(easyqt::DataStorage::get()->resourceFile("poop", easyqt::ResourceType::Icon))
    , _icon(_no_icon)
    , _network(new QNetworkAccessManager(this))
    , _timerUpdateDayMode(new QTimer(this))
    , timer_update_weather_(new QTimer(this))
{
    for (int i = 0; i < 24; ++i)
    {
        _forecast.append(new ForecastData(this));
    }

    Preferences::access()->registerPreference(PreferenceEntry::WeatherLatitude, QMetaType::Double, 0.0);
    Preferences::access()->registerPreference(PreferenceEntry::WeatherLongitude, QMetaType::Double, 0.0);
    Preferences::access()->registerPreference(PreferenceEntry::WeatherAPIKey, QMetaType::QString, "");

    connect(timer_update_weather_, &QTimer::timeout, this, &WeatherAccessor::updateWeather);
    timer_update_weather_->setSingleShot(true);

    _timerUpdateDayMode->setSingleShot(true);
    connect(_timerUpdateDayMode, &QTimer::timeout, this, &WeatherAccessor::updateDayMode);

    updateWeather();
}

QQmlListProperty<ForecastData> WeatherAccessor::getForecast()
{
    return QQmlListProperty<ForecastData>(this, &_forecast);
}

void WeatherAccessor::updateWeather()
{
    // Current weather
    {
        QUrl url("https://api.openweathermap.org/data/4.0/onecall/current");

        setupRequest(url);

        QNetworkReply* reply = _network->get(QNetworkRequest(url));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() { onRequestCurrentFinished(reply); });
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    }

    // Forecast
    {
        QUrl url("https://api.openweathermap.org/data/4.0/onecall/timeline/1h");

        setupRequest(url);

        QNetworkReply* reply = _network->get(QNetworkRequest(url));
        connect(reply, &QNetworkReply::finished, this, [this, reply]() { onRequestForecastFinished(reply); });
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    }
}

void WeatherAccessor::updateDayMode()
{
    _timerUpdateDayMode->stop();

    DayMode::Enum newDayMode = DayMode::Unknown;
    if (_sunrise.isValid() && _sunset.isValid())
    {
        QDateTime now = QDateTime::currentDateTimeUtc();
        newDayMode = (now <= _sunrise || now >= _sunset) ? DayMode::Night : DayMode::Day;

        if (now < _sunrise)
        {
            _timerUpdateDayMode->start(now.msecsTo(_sunrise) + 1);
        }
        else if (now < _sunset)
        {
            _timerUpdateDayMode->start(now.msecsTo(_sunset) + 1);
        }
        else
        {
            // Way for next day data !
        }
    }

    if (newDayMode != _dayMode)
    {
        _dayMode = newDayMode;
        emit dayModeChanged(_dayMode);
    }
}

void WeatherAccessor::onRequestCurrentFinished(QNetworkReply* reply)
{
    _temperature = Constants::empty;
    _icon = _no_icon;

    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &error);
        if (error.error == QJsonParseError::NoError)
        {
            QJsonValue valueData = doc["data"].toArray().at(0);
            _temperature = QString("%1°").arg(valueData["temp"].toDouble(), 0, 'f', 1);
            _sunrise = QDateTime::fromSecsSinceEpoch(valueData["sunrise"].toInt(), Qt::UTC);
            _sunset = QDateTime::fromSecsSinceEpoch(valueData["sunset"].toInt(), Qt::UTC);
            _icon = getIcon(valueData);

            updateDayMode();
        }
        else
        {
            qWarning() << "JSON parsing error :" << error.errorString();
        }
    }
    else
    {
        qWarning() << "Network error on receiving current weather data :" << reply->errorString();
    }

    timer_update_weather_->start(5min);

    emit currentDataChanged();
}

void WeatherAccessor::onRequestForecastFinished(QNetworkReply* reply)
{
    for (ForecastData* forecast_data : _forecast)
    {
        forecast_data->set(0.0, 0.0, QString());
    }

    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &error);
        if (error.error == QJsonParseError::NoError)
        {
            qint64 now = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
            QJsonArray value_data = doc["data"].toArray();

            qsizetype forecast_index = 0;
            for (qsizetype i = 0; i < value_data.size() && forecast_index < _forecast.size(); ++i)
            {
                QJsonValue forecast_value = value_data.at(i);
                double timestamp = forecast_value["dt"].toDouble();
                if (timestamp > now)
                {
                    _forecast[forecast_index++]->set(
                        timestamp,
                        forecast_value["temp"].toDouble(),
                        getIcon(forecast_value));
                }
            }
        }
        else
        {
            qWarning() << "JSON parsing error :" << error.errorString();
        }
    }
    else
    {
        qWarning() << "Network error on receiving forecast data :" << reply->errorString();
    }

    emit forecastChanged();
}

void WeatherAccessor::setupRequest(QUrl& url)
{
    const Preferences& preferences = *Preferences::get();

    QUrlQuery query;
    query.addQueryItem("lat", QString::number(preferences.getDouble(PreferenceEntry::WeatherLatitude), 'f', 6));
    query.addQueryItem("lon", QString::number(preferences.getDouble(PreferenceEntry::WeatherLongitude), 'f', 6));
    query.addQueryItem("appid", preferences.getString(PreferenceEntry::WeatherAPIKey));
    query.addQueryItem("units", "metric");

    url.setQuery(query);
}

QString WeatherAccessor::getIcon(const QJsonValue& value) const
{
    const QString raw_icon = value["weather"].toArray().at(0)["icon"].toString();

    QString local_icon;
    if (raw_icon == "01d")
    {
        local_icon = "sun";
    }
    else if (raw_icon == "01n")
    {
        local_icon = "night";
    }
    else if (raw_icon == "02d")
    {
        local_icon = "cloudy-day";
    }
    else if (raw_icon == "02n")
    {
        local_icon = "cloudy-night";
    }
    else if (raw_icon.startsWith("03"))
    {
        local_icon = "cloud";
    }
    else if (raw_icon.startsWith("04"))
    {
        local_icon = "cloud-dark";
    }
    else if (raw_icon.startsWith("09"))
    {
        local_icon = "rain";
    }
    else if (raw_icon == "10d")
    {
        local_icon = "rain-day";
    }
    else if (raw_icon == "10n")
    {
        local_icon = "rain-night";
    }
    else if (raw_icon.startsWith("11"))
    {
        local_icon = "thunder";
    }
    else if (raw_icon.startsWith("13"))
    {
        local_icon = "snow";
    }
    else if (raw_icon == "50d")
    {
        local_icon = "foggy";
    }
    else if (raw_icon == "50n")
    {
        local_icon = "foggy-night";
    }
    else
    {
        return _no_icon;
    }

    return easyqt::DataStorage::get()->resourceFile(local_icon, easyqt::ResourceType::Icon, "weather");
}
