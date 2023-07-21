#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QVariant>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QEventLoop>

#include <qqml.h>

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

#include "datetime.h"
#include "tasksloader.h"
#include "jsonreader.h"

class Weather : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(double temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(double humidity READ humidity WRITE setHumidity NOTIFY humidityChanged)
    Q_PROPERTY(double windSpeed READ windSpeed WRITE setWindSpeed NOTIFY windSpeedChanged)
    Q_PROPERTY(double uvIndex READ uvIndex WRITE setUvIndex NOTIFY uvIndexChanged)
    Q_PROPERTY(int airPressure READ airPressure WRITE setAirPressure NOTIFY airPressureChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString statusIconUrl READ statusIconUrl WRITE setStatusIconUrl NOTIFY statusIconUrlChanged)
    Q_PROPERTY(DateTime *dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(TasksLoader *tasksLoader READ tasksLoader WRITE setTasksLoader NOTIFY tasksLoaderChanged)
    Q_PROPERTY(int currentDay READ currentDay WRITE setCurrentDay NOTIFY currentDayChanged)
    Q_PROPERTY(int currentHourInDay READ currentHourInDay WRITE setCurrentHourInDay NOTIFY currentHourInDayChanged)
    Q_PROPERTY(QStringList weatherIconPaths READ weatherIconPaths WRITE setWeatherIconPaths NOTIFY weatherIconPathsChanged)
    QML_ELEMENT

public:
    explicit Weather(QObject *parent = nullptr);

    void sendHttpRequest(QNetworkAccessManager* networkManager, QUrl url);

    QString reformatCityToUrlCity(QString city);

    Q_INVOKABLE void reloadWeatherFromLocation(QString city);

    QString getWeatherStatusFromCode(QJsonObject weatherJsonObject);
    QString getWeatherIconUrlFromCode(QJsonObject weatherJsonObject, int scale);

    void setWeatherIconPathsFromEachHour(QJsonObject weatherJsonObject);

    void getCurrentHourFromCurrentHourInDay();

    double kelvin2Celsius(double kevinTemperature);

    template <typename T>
    std::string number2StdString(T number);

    void setWeatherInfo(QJsonObject weatherJsonData);
    void setCityInfo(QJsonObject cityJsonData);

    void reformatStatusText();

    const QString &status() const;
    void setStatus(const QString &newStatus);

    double temperature() const;
    void setTemperature(double newTemperature);

    double humidity() const;
    void setHumidity(double newHumidity);

    double windSpeed() const;
    void setWindSpeed(double newWindSpeed);

    double uvIndex() const;
    void setUvIndex(double newUvIndex);

    int airPressure() const;
    void setAirPressure(int newAirPressure);

    const QString &location() const;
    void setLocation(const QString &newLocation);

    const QString &statusIconUrl() const;
    void setStatusIconUrl(const QString &newStatusIconName);

    DateTime *dateTime() const;
    void setDateTime(DateTime *newDateTime);

    TasksLoader *tasksLoader() const;
    void setTasksLoader(TasksLoader *newTasksLoader);

    int currentDay() const;
    void setCurrentDay(int newCurrentDay);

    int currentHourInDay() const;
    void setCurrentHourInDay(int newCurrentHourInDay);

    QStringList weatherIconPaths() const;
    void setWeatherIconPaths(const QStringList &newWeatherIconPaths);

signals:

    void statusChanged();

    void temperatureChanged();

    void humidityChanged();

    void windSpeedChanged();

    void uvIndexChanged();

    void airPressureChanged();

    void locationChanged();

    void statusIconUrlChanged();

    void dateTimeChanged();

    void tasksLoaderChanged();

    void currentDayChanged();

    void currentHourInDayChanged();

    void weatherIconPathsChanged();

private:
    TasksLoader* m_tasksLoader;

    QString m_status; // The description about the weather
    double m_temperature;
    double m_humidity;
    double m_windSpeed;
    double m_uvIndex;
    int m_airPressure;

    int m_currentDay;

    /* For example, if currentHour = 25 then the currentHourInDay = 1, understand? */
    int m_currentHourInDay;
    int m_currentHour;

    QString m_statusIconUrl;

    QString m_location;
    float m_latitude, m_longitude;

    QString m_currentTimezone;

    DateTime* m_dateTime;

    JsonReader m_jsonReader;

    bool m_currentTimeIsDay;

    QStringList m_weatherIconPaths;

    QJsonObject m_oldWeatherJsonData;

    QNetworkAccessManager* m_weatherNetworkManager;
    QNetworkAccessManager* m_cityCoordNetworkManager;

    QNetworkRequest m_networkRequest;

    const std::string apiKey = "9ae7907c3148b138968051b92b60f5e0";
    std::string m_apiURL;
};

#endif // WEATHER_H
