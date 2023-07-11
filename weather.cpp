#include "weather.h"

Weather::Weather(QObject *parent)
    : QObject{parent}
{
    m_cityCoordNetworkManager = new QNetworkAccessManager();
    m_weatherNetworkManager = new QNetworkAccessManager();

    reloadWeatherFromLocation("Houston");

    connect(m_cityCoordNetworkManager, &QNetworkAccessManager::finished,
    this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            setCoordsAndTimezoneFromLocation(reply);
        }

        else {
            qDebug() << reply->errorString();
            return;
        }
    });

    connect(m_weatherNetworkManager, &QNetworkAccessManager::finished,
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

    sendHttpRequest(m_cityCoordNetworkManager,
                    QUrl("https://geocoding-api.open-meteo.com/v1/search?name=" + location() + "&count=1&language=en&format=json"));

}

double Weather::kelvin2Celsius(double kevinTemperature)
{
    return kevinTemperature - 273.15;
}

std::string Weather::number2StdString(auto number)
{
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

double Weather::roundTemperature(double temp)
{
    double resultNum = floor(temp * 10.0) / 10.0;

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

QJsonObject Weather::readJsonNetworkReply(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    return jsonResponse.object();
}

void Weather::setWeatherProperties(QNetworkReply *reply)
{
    QJsonObject jsonObject = readJsonNetworkReply(reply);

    double temperature = jsonObject["hourly"].toObject()["temperature_2m"].toArray()[0].toDouble();
    temperature = roundTemperature(temperature);
    int humidity = jsonObject["hourly"].toObject()["relativehumidity_2m"].toArray()[0].toInt();
    QString weatherDescription = jsonObject["weather"].toArray()[0].toObject()["description"].toString();
    double windSpeedkmh = jsonObject["hourly"].toObject()["windspeed_10m"].toArray()[0].toDouble();;

    setStatus(weatherDescription);
    setWindSpeed(windSpeedkmh);
    setTemperature(temperature);
    setHumidity(humidity);

    tasksLoader()->setTemperature(temperature);
    tasksLoader()->setWeatherStatus(weatherDescription);
    tasksLoader()->setTasksList(tasksLoader()->getSuitableTasksWithWeather());

    reformatStatusText();
}

void Weather::setCoordsAndTimezoneFromLocation(QNetworkReply* reply)
{
    QJsonObject jsonObject = readJsonNetworkReply(reply);

    float latitude = jsonObject["results"].toArray()[0].toObject()["latitude"].toDouble();
    float longitude = jsonObject["results"].toArray()[0].toObject()["longitude"].toDouble();

    m_latitude = latitude;
    m_longitude = longitude;

    m_apiURL = "https://api.open-meteo.com/v1/forecast?"
               "latitude=" + number2StdString(latitude) + "&longitude=" + number2StdString(longitude) +
               "&hourly=temperature_2m,relativehumidity_2m,rain,weathercode,visibility,windspeed_10m,uv_index"
               "&daily=weathercode&timezone=America%2FChicago";

    sendHttpRequest(m_weatherNetworkManager, QUrl(QString::fromUtf8(m_apiURL.c_str())));
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
