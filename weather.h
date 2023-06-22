#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <qqml.h>

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>

class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(double temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(double humidity READ humidity WRITE setHumidity NOTIFY humidityChanged)
    Q_PROPERTY(double windSpeed READ windSpeed WRITE setWindSpeed NOTIFY windSpeedChanged)
    Q_PROPERTY(int uvIndex READ uvIndex WRITE setUvIndex NOTIFY uvIndexChanged)
    Q_PROPERTY(int airPressure READ airPressure WRITE setAirPressure NOTIFY airPressureChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString statusIconPath READ statusIconPath WRITE setStatusIconPath NOTIFY statusIconPathChanged)
    QML_ELEMENT

public:
    explicit Weather(QObject *parent = nullptr);

    QString reformatCityToUrlCity(QString city);

    Q_INVOKABLE void reloadWeatherFromLocation(QString city);

    double kelvin2Celsius(double kevinTemperature);

    double roundTo1Decimal(double number);

    void setWeatherProperties();
    void reformatStatusText();
    void getSuitableIconFromStatus();

    static size_t WriteCallbackAPI(void* contents, size_t size, size_t nmemb, std::string* data);

    const QString &status() const;
    void setStatus(const QString &newStatus);

    double temperature() const;
    void setTemperature(double newTemperature);

    double humidity() const;
    void setHumidity(double newHumidity);

    double windSpeed() const;
    void setWindSpeed(double newWindSpeed);

    int uvIndex() const;
    void setUvIndex(int newUvIndex);

    int airPressure() const;
    void setAirPressure(int newAirPressure);

    const QString &location() const;
    void setLocation(const QString &newLocation);

    const QString &statusIconPath() const;
    void setStatusIconPath(const QString &newStatusIconPath);

signals:

    void statusChanged();

    void temperatureChanged();

    void humidityChanged();

    void windSpeedChanged();

    void uvIndexChanged();

    void airPressureChanged();

    void locationChanged();

    void statusIconPathChanged();

private:
    QString m_status; // The description about the weather
    double m_temperature;
    double m_humidity;
    double m_windSpeed;
    int m_uvIndex;
    int m_airPressure;

    QString m_statusIconPath;

    QString m_location;

    const std::string apiKey = "9ae7907c3148b138968051b92b60f5e0";
    std::string url;
};

#endif // WEATHER_H
