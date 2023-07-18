#include "weather.h"

Weather::Weather(QObject *parent)
    : QObject{parent}
{
    m_currentDay = 0; // 0 is current day, 1 is tomorrow,...
    m_currentHourInDay = dateTime()->getCurrentRoundedTime().split(":")[0].toInt();

    m_cityCoordNetworkManager = new QNetworkAccessManager();
    m_weatherNetworkManager = new QNetworkAccessManager();

    reloadWeatherFromLocation("Houston");

    connect(m_cityCoordNetworkManager, &QNetworkAccessManager::finished,
    this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            setCityInfo(reply);
        }

        else {
            qDebug() << reply->errorString();
            return;
        }
    });

    connect(m_weatherNetworkManager, &QNetworkAccessManager::finished,
    this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            setWeatherInfo(reply);
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
    getCurrentHourFromCurrentHourInDay();
    city = reformatCityToUrlCity(city);

    setLocation(city);

    sendHttpRequest(m_cityCoordNetworkManager,
        QUrl("https://geocoding-api.open-meteo.com/v1/search?name=" + location() + "&count=1&language=en&format=json"));

}

QString Weather::getWeatherStatusFromCode(int weatherCode)
{
    QJsonObject statusWeatherCode = m_jsonReader.readJsonFile(":/statusWeatherCode.json");
    return statusWeatherCode[QString::number(weatherCode)]
        .toObject()[( m_timeIsDay ? "day" : "night")].toObject()["description"].toString();
}

QString Weather::getWeatherIconUrlFromCode(int weatherCode, int scale)
{
    QJsonObject statusWeatherCode = m_jsonReader.readJsonFile(":/statusWeatherCode.json");
    return statusWeatherCode[QString::number(weatherCode)]
        .toObject()[( m_timeIsDay ? "day" : "night")].
        toObject()["image"].toString().replace("4x", QString::number(scale) + "x");
}

void Weather::setWeatherIconPathsFromEachHour(QJsonArray weatherCodeJson)
{
    // We will "borrow" the attribute m_currentHourInDay and m_currentHour to do it, then give them back
    int currentHourInDayTemp = m_currentHourInDay;
    int currentHourTemp = m_currentHour;

    m_weatherIconPaths.clear();

    for (m_currentHourInDay = 0; m_currentHourInDay < 24; m_currentHourInDay++) {
        getCurrentHourFromCurrentHourInDay();
        QString currentPath = getWeatherIconUrlFromCode(weatherCodeJson[m_currentHour].toInt(), 2);
        m_weatherIconPaths.append(currentPath);
    }

    // Give their data back
    m_currentHourInDay = currentHourInDayTemp;
    m_currentHour = currentHourTemp;
}

void Weather::getCurrentHourFromCurrentHourInDay()
{
   m_currentHour = m_currentHourInDay + m_currentDay * 24;

}

double Weather::kelvin2Celsius(double kevinTemperature)
{
    return kevinTemperature - 273.15;
}

template <typename T>
std::string Weather::number2StdString(T number)
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

void Weather::setWeatherInfo(QNetworkReply *reply)
{
    QJsonObject jsonObject = m_jsonReader.readJsonNetworkReply(reply);

    qDebug() <<  m_currentHour;

    double temperature = jsonObject["hourly"].toObject()["temperature_2m"].toArray()[m_currentHour].toDouble();
    temperature = roundTemperature(temperature);
    int humidity = jsonObject["hourly"].toObject()["relativehumidity_2m"].toArray()[m_currentHour].toInt();
    int weatherCode = jsonObject["hourly"].toObject()["weathercode"].toArray()[m_currentHour].toInt();
    double windSpeedkmh = jsonObject["hourly"].toObject()["windspeed_10m"].toArray()[m_currentHour].toDouble();
    double uvIndex = jsonObject["hourly"].toObject()["uv_index"].toArray()[m_currentHour].toDouble();
    m_timeIsDay = !!jsonObject["hourly"].toObject()["is_day"].toArray()[m_currentHour].toInt();

    setWeatherIconPathsFromEachHour(jsonObject["hourly"].toObject()["weathercode"].toArray());

    QString weatherStatus = getWeatherStatusFromCode(weatherCode);
    QString weatherIconPath = getWeatherIconUrlFromCode(weatherCode, 4);

    setWindSpeed(windSpeedkmh);
    setTemperature(temperature);
    setHumidity(humidity);
    setUvIndex(uvIndex);
    setStatusIconUrl(weatherIconPath);
    setStatus(weatherStatus);

    tasksLoader()->setTemperature(temperature);
    tasksLoader()->setWeatherStatus(weatherStatus);
    tasksLoader()->setTasksList(tasksLoader()->getSuitableTasksWithWeather());

    reformatStatusText();
}

void Weather::setCityInfo(QNetworkReply* reply)
{
    QJsonObject jsonObject = m_jsonReader.readJsonNetworkReply(reply);

    float latitude = jsonObject["results"].toArray()[0].toObject()["latitude"].toDouble();
    float longitude = jsonObject["results"].toArray()[0].toObject()["longitude"].toDouble();

    m_timeIsDay = jsonObject["results"].toArray()[0].toObject()["latitude"].toDouble();
    m_timezone = jsonObject["results"].toArray()[0].toObject()["timezone"].toString();

    m_latitude = latitude;
    m_longitude = longitude;

    m_apiURL =  "https://api.open-meteo.com/v1/forecast?"
                "latitude=" + number2StdString(latitude) + "&longitude=" + number2StdString(longitude) +
                "&hourly=temperature_2m,relativehumidity_2m,rain,weathercode,visibility,windspeed_10m,uv_index,is_day"
                "&daily=weathercode&timezone=" + m_timezone.toStdString();

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
        setStatusIconUrl("sunny");
    }

    if (loweredStatusString.contains("rain")) // light or heavy rain
        setStatusIconUrl("rainy");

    else if (loweredStatusString == "broken clouds" ||
             loweredStatusString == "few clouds" ||
             loweredStatusString == "scattered clouds")
        setStatusIconUrl("few-clouds");

    else if (loweredStatusString.contains("snow"))
        setStatusIconUrl("snowy");

    else if (loweredStatusString.contains("clouds"))
        setStatusIconUrl("cloudy");
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

double Weather::uvIndex() const
{
    return m_uvIndex;
}

void Weather::setUvIndex(double newUvIndex)
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

void Weather::setStatusIconUrl(const QString &newStatusIconUrl)
{
    if (m_statusIconUrl == newStatusIconUrl)
        return;
    m_statusIconUrl = newStatusIconUrl;
    emit statusIconUrlChanged();
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

int Weather::currentDay() const
{
    return m_currentDay;
}

void Weather::setCurrentDay(int newCurrentDay)
{
    if (m_currentDay == newCurrentDay)
        return;
    m_currentDay = newCurrentDay;
    emit currentDayChanged();
}

int Weather::currentHourInDay() const
{
    return m_currentHourInDay;
}

void Weather::setCurrentHourInDay(int newCurrentHourInDay)
{
    if (m_currentHourInDay == newCurrentHourInDay)
        return;
    m_currentHourInDay = newCurrentHourInDay;
    emit currentHourInDayChanged();
}

QStringList Weather::weatherIconPaths() const
{
    return m_weatherIconPaths;
}

void Weather::setWeatherIconPaths(const QStringList &newWeatherIconPaths)
{
    if (m_weatherIconPaths == newWeatherIconPaths)
        return;
    m_weatherIconPaths = newWeatherIconPaths;
    emit weatherIconPathsChanged();
}
