#include "weather.h"

Weather::Weather(QObject *parent)
    : QObject{parent}
{
    m_status = "Loading info...";
    m_temperature = 0;
    m_humidity = 0;
    m_rain = 0;
    m_windSpeed = 0;
    m_uvIndex = 0;

    m_currentDay = 0; // 0 is current day, 1 is tomorrow,...
    m_currentHourInDay = dateTime()->getCurrentRoundedTime().split(":")[0].toInt();

    m_cityCoordNetworkManager = new QNetworkAccessManager();
    m_weatherNetworkManager = new QNetworkAccessManager();

    reloadWeatherFromLocation("Houston");

    connect(m_cityCoordNetworkManager, &QNetworkAccessManager::finished,
    this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            setCityInfo(JsonReader::readJsonNetworkReply(reply));
        }

        else {
            qDebug() << reply->errorString();
            return;
        }
    });

    connect(m_weatherNetworkManager, &QNetworkAccessManager::finished,
    this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            setWeatherInfo(JsonReader::readJsonNetworkReply(reply));
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
    getCurrentHourFromCurrentHourInDay();

    if (city != m_location) {
        setLocation(city);

        sendHttpRequest(m_cityCoordNetworkManager,
                        QUrl("https://geocoding-api.open-meteo.com/v1/search?name=" +
                            location() + "&count=1&language=en&format=json"));
    } else {
        setWeatherInfo(m_oldWeatherJsonData);
        qDebug() << "Code ran";
    }

}

QString Weather::convertWeatherCodeToWeatherStatus(QString weatherCode, bool timeIsDay)
{
    QJsonObject weatherCodeDict = JsonReader::readJsonFile(":/statusWeatherCode.json");

    return weatherCodeDict[weatherCode]
        .toObject()[( timeIsDay ? "day" : "night")].toObject()["description"].toString();
}

QString Weather::convertWeatherCodeToWeatherUrl(QString weatherCode, bool timeIsDay)
{
    QJsonObject weatherCodeDict = JsonReader::readJsonFile(":/statusWeatherCode.json");

    return weatherCodeDict[weatherCode]
        .toObject()[( timeIsDay ? "day" : "night")].toObject()["image"].toString();
}

QString Weather::getWeatherStatusFromCode(QJsonObject weatherJsonObject)
{
    QString weatherCode = QString::number(weatherJsonObject["hourly"]
                                              .toObject()["weathercode"].toArray()[m_currentHour].toInt());

    bool timeIsDay = !!weatherJsonObject["hourly"].toObject()["is_day"].toArray()[m_currentHour].toInt();

    return convertWeatherCodeToWeatherStatus(weatherCode, timeIsDay);
}

QString Weather::getWeatherIconUrlFromCode(QJsonObject weatherJsonObject, int scale)
{
    QString weatherCode = QString::number(weatherJsonObject["hourly"]
                                              .toObject()["weathercode"].toArray()[m_currentHour].toInt());

    bool timeIsDay = !!weatherJsonObject["hourly"].toObject()["is_day"].toArray()[m_currentHour].toInt();

    return convertWeatherCodeToWeatherUrl(weatherCode, timeIsDay);
}

void Weather::setWeatherIconPathsFromEachHour(QJsonObject weatherJsonObject)
{
    // We will "borrow" the attribute m_currentHourInDay and m_currentHour to do it, then give them back
    int currentHourInDayTemp = m_currentHourInDay;
    int currentHourTemp = m_currentHour;

    m_weatherIconPaths.clear();

    for (m_currentHourInDay = 0; m_currentHourInDay < 24; m_currentHourInDay++) {
        getCurrentHourFromCurrentHourInDay();
        QString currentPath = getWeatherIconUrlFromCode(weatherJsonObject, 2);
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

QStringList Weather::getDailyWeatherIconUrlList()
{
    QStringList result;

    QString weatherCode;
    QString weatherUrlList;

    for (int day = 0; day < 7; day++) {
        weatherCode = QString::number(m_oldWeatherJsonData["daily"].toObject()["weathercode"].toArray()[day].toInt());
        weatherUrlList = convertWeatherCodeToWeatherUrl(weatherCode, m_currentTimeIsDay);
        result.append(weatherUrlList);
    }

    return result;
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

void Weather::setWeatherInfo(QJsonObject weatherJsonData)
{
    m_oldWeatherJsonData = weatherJsonData;

    double temperature = weatherJsonData["hourly"].toObject()["temperature_2m"].toArray()[m_currentHour].toDouble();
    temperature = round(temperature);
    int humidity = weatherJsonData["hourly"].toObject()["relativehumidity_2m"].toArray()[m_currentHour].toInt();
    int weatherCode = weatherJsonData["hourly"].toObject()["weathercode"].toArray()[m_currentHour].toInt();
    double windSpeedkmh = weatherJsonData["hourly"].toObject()["windspeed_10m"].toArray()[m_currentHour].toDouble();
    double uvIndex = weatherJsonData["hourly"].toObject()["uv_index"].toArray()[m_currentHour].toDouble();
    double rain = weatherJsonData["hourly"].toObject()["rain"].toArray()[m_currentHour].toDouble();
    m_currentTimeIsDay = !!weatherJsonData["hourly"].toObject()["is_day"].toArray()[m_currentHour].toInt();

    setWeatherIconPathsFromEachHour(weatherJsonData);

    qDebug() << QString::fromUtf8(m_apiURL.c_str());

    QString weatherStatus = getWeatherStatusFromCode(weatherJsonData);
    QString weatherIconPath = getWeatherIconUrlFromCode(weatherJsonData, 4);

    setWindSpeed(windSpeedkmh);
    setTemperature(temperature);
    setHumidity(humidity);
    setUvIndex(uvIndex);
    setStatusIconUrl(weatherIconPath);
    setStatus(weatherStatus);
    setRain(rain);

    tasksLoader()->setTemperature(temperature);
    tasksLoader()->setWeatherStatus(weatherStatus);
    tasksLoader()->setTasksList(tasksLoader()->getSuitableTasksWithWeather());

    reformatStatusText();
}

void Weather::setCityInfo(QJsonObject cityJsonData)
{
    float latitude = cityJsonData["results"].toArray()[0].toObject()["latitude"].toDouble();
    float longitude = cityJsonData["results"].toArray()[0].toObject()["longitude"].toDouble();

    m_currentTimezone = cityJsonData["results"].toArray()[0].toObject()["timezone"].toString();

    m_latitude = latitude;
    m_longitude = longitude;

    m_apiURL =  "https://api.open-meteo.com/v1/forecast?"
                "latitude=" + number2StdString(latitude) + "&longitude=" + number2StdString(longitude) +
                "&hourly=temperature_2m,relativehumidity_2m,rain,weathercode,visibility,windspeed_10m,uv_index,is_day"
                "&daily=weathercode&timezone=" + m_currentTimezone.toStdString();

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

double Weather::rain() const
{
    return m_rain;
}

void Weather::setRain(double newRain)
{
    if (qFuzzyCompare(m_rain, newRain))
        return;
    m_rain = newRain;
    emit rainChanged();
}
