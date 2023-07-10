#include "weather.h"

Weather::Weather(QObject *parent)
    : QObject{parent}
{
    m_networkManager = new QNetworkAccessManager();

    reloadWeatherFromLocation("Texas");

    connect(m_networkManager, &QNetworkAccessManager::finished,
    this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            setWeatherProperties(reply);
        }

        else {
            qDebug() << reply->errorString();
            return;
        }
    });
}

void Weather::sendHttpRequest(QNetworkAccessManager *networkManager, QUrl url)
{
    m_networkRequest.setUrl(url);
    networkManager->get(m_networkRequest);
}

QString Weather::reformatCityToUrlCity(QString city)
{
    QString result;
    if (city.contains("Hue"))
        result = "Hue";

    else {
        for (int i = 0; i < city.length(); i++) {
            if (city[i] == ' ')
                result += "%20";
            else
                result += city[i];

            if (city.contains(",") && city[i+1] == ',')
                break;
        }
    }

    return result;
}

void Weather::reloadWeatherFromLocation(QString city)
{
    city = reformatCityToUrlCity(city);

    setLocation(city);
    m_apiURL = ("http://api.openweathermap.org/data/2.5/weather?q="
           + location().toStdString() + "&appid=" + apiKey);

    sendHttpRequest(m_networkManager, QUrl(QString::fromUtf8(m_apiURL.c_str())));
}

double Weather::kelvin2Celsius(double kevinTemperature)
{
    return kevinTemperature - 273.15;
}

double Weather::roundTo1Decimal(double number)
{
    double resultNum = floor(number * 10.0) / 10.0;

    QString decimalToCheck = QString::number(resultNum);
    if (decimalToCheck.contains("."))
        decimalToCheck = decimalToCheck.split(".")[1];
    else
        return resultNum;

    if (decimalToCheck == '6' || decimalToCheck == '1')
        resultNum -= 0.1;
    if (decimalToCheck == '9' || decimalToCheck == '4')
        resultNum += 0.1;

    return resultNum;
}

void Weather::setWeatherProperties(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    QJsonObject jsonObject = jsonResponse.object();

    double temperature = roundTo1Decimal(kelvin2Celsius(jsonObject["main"].toObject()["temp"].toDouble()));
    int humidity = jsonObject["main"].toObject()["humidity"].toInt();
    QString weatherDescription = jsonObject["weather"].toArray()[0].toObject()["description"].toString();
    double windSpeedms = jsonObject["wind"].toObject()["speed"].toDouble();
    double windSpeedkmh = roundTo1Decimal(windSpeedms * 3.5);
    double airPressure = jsonObject["main"].toObject()["pressure"].toDouble();
    QString weatherIconName = jsonObject["weather"].toArray()[0].toObject()["icon"].toString();

    setStatus(weatherDescription);
    setAirPressure(airPressure);
    setWindSpeed(windSpeedkmh);
    setTemperature(temperature);
    setHumidity(humidity);
    setStatusIconName(weatherIconName);

    tasksLoader()->setTemperature(temperature);
    tasksLoader()->setWeatherStatus(weatherDescription);
    tasksLoader()->setTasksList(tasksLoader()->getSuitableTasksWithWeather());

    reformatStatusText();
}

void Weather::reformatStatusText()
{
    /* Reformat? just make the first letter of each word to uppercase (by default,
     They are lowercase) */

    QString resultStringStatus = status();
    resultStringStatus[0] = resultStringStatus[0].toUpper();
    for (int i = 0; i < status().length(); i++)
    {
        if (status()[i] == " ") {
            resultStringStatus[i+1] = resultStringStatus[i+1].toUpper();
        }
    }
    setStatus(resultStringStatus);
}

void Weather::getSuitableIconFromStatus()
{
    QString loweredStatusString = status().toLower();

    if (loweredStatusString == "clear sky") {
        setStatusIconName("sunny");
    }

    if (loweredStatusString.contains("rain")) // light or heavy rain
        setStatusIconName("rainy");

    else if (loweredStatusString == "broken clouds" ||
             loweredStatusString == "few clouds" ||
             loweredStatusString == "scattered clouds")
        setStatusIconName("few-clouds");

    else if (loweredStatusString.contains("snow"))
        setStatusIconName("snowy");

    else if (loweredStatusString.contains("clouds"))
        setStatusIconName("cloudy");
}

const QString &Weather::status() const
{
    return m_status;
}

void Weather::setStatus(const QString &newStatus)
{
    if (m_status == newStatus)
        return;
    m_status = newStatus;
    emit statusChanged();
}

double Weather::temperature() const
{
    return m_temperature;
}

void Weather::setTemperature(double newTemperature)
{
    if (qFuzzyCompare(m_temperature, newTemperature))
        return;
    m_temperature = newTemperature;
    emit temperatureChanged();
}

double Weather::humidity() const
{
    return m_humidity;
}

void Weather::setHumidity(double newHumidity)
{
    if (m_humidity == newHumidity)
        return;
    m_humidity = newHumidity;
    emit humidityChanged();
}

double Weather::windSpeed() const
{
    return m_windSpeed;
}

void Weather::setWindSpeed(double newWindSpeed)
{
    if (qFuzzyCompare(m_windSpeed, newWindSpeed))
        return;
    m_windSpeed = newWindSpeed;
    emit windSpeedChanged();
}

int Weather::uvIndex() const
{
    return m_uvIndex;
}

void Weather::setUvIndex(int newUvIndex)
{
    if (m_uvIndex == newUvIndex)
        return;
    m_uvIndex = newUvIndex;
    emit uvIndexChanged();
}

int Weather::airPressure() const
{
    return m_airPressure;
}

void Weather::setAirPressure(int newAirPressure)
{
    if (m_airPressure == newAirPressure)
        return;
    m_airPressure = newAirPressure;
    emit airPressureChanged();
}

const QString &Weather::location() const
{
    return m_location;
}

void Weather::setLocation(const QString &newLocation)
{
    if (m_location == newLocation)
        return;
    m_location = newLocation;
    emit locationChanged();
}

const QString &Weather::statusIconUrl() const
{
    return m_statusIconUrl;
}

void Weather::setStatusIconName(const QString &newStatusIconName)
{
    if (m_statusIconUrl == newStatusIconName)
        return;
    m_statusIconUrl = "https://openweathermap.org/img/wn/" + newStatusIconName + "@4x.png";
    emit statusIconNameChanged();
}

DateTime *Weather::dateTime() const
{
    return m_dateTime;
}

void Weather::setDateTime(DateTime *newDateTime)
{
    if (m_dateTime == newDateTime)
        return;
    m_dateTime = newDateTime;
    emit dateTimeChanged();
}

TasksLoader *Weather::tasksLoader() const
{
    return m_tasksLoader;
}

void Weather::setTasksLoader(TasksLoader *newTasksLoader)
{
    if (m_tasksLoader == newTasksLoader)
        return;
    m_tasksLoader = newTasksLoader;
    emit tasksLoaderChanged();
}
