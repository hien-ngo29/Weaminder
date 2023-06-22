#include "weather.h"

Weather::Weather(QObject *parent)
    : QObject{parent}
{
    reloadWeatherFromLocation("Texas");
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
    url = ("http://api.openweathermap.org/data/2.5/weather?q="
           + location().toStdString() + "&appid=" + apiKey);
    setWeatherProperties();
}

double Weather::kelvin2Celsius(double kevinTemperature)
{
    return kevinTemperature - 273.15;
}

double Weather::roundTo1Decimal(double number)
{
    double resultNum = floor(number * 10.0) / 10.0;

    std::ostringstream strs;
    strs << resultNum;
    std::string resultString = strs.str();

    char decimalToCheck;
    for (int i = 0; i < resultString.length(); i++) {
        if (resultString[i] == '.') {
            decimalToCheck = resultString[i+1];
            break;
        }
    }

    qDebug() << decimalToCheck;

    if (decimalToCheck == '6' || decimalToCheck == '1')
        resultNum -= 0.1;
    if (decimalToCheck == '9' || decimalToCheck == '4')
        resultNum += 0.1;

    return resultNum;
}

void Weather::setWeatherProperties()
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to handle the response
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackAPI);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode result = curl_easy_perform(curl);

        // Check for errors
        if (result != CURLE_OK)
        {
            qDebug() << "Request failed: " << curl_easy_strerror(result);
            curl_easy_cleanup(curl);
        }

        // Parse the response as JSON
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::istringstream iss(response);
        std::string parseErrors;
        if (!Json::parseFromStream(reader, iss, &root, &parseErrors))
        {
            qDebug() << "Failed to parse JSON: ";
            curl_easy_cleanup(curl);
        }

        // Extract and display weather information
        double temperatureKelvin = root["main"]["temp"].asDouble();
        double temperatureCelsius = roundTo1Decimal(kelvin2Celsius(temperatureKelvin));
        double humidityLevel = roundTo1Decimal(root["main"]["humidity"].asDouble());
        double uvIndexLevel = roundTo1Decimal(root["uv_index"].asDouble());
        double windSpeedms = root["wind"]["speed"].asDouble();
        double windSpeedkmh = roundTo1Decimal(windSpeedms * 3.6);
        double airPressureLevel = root["main"]["pressure"].asFloat();
        std::string weatherDescription = root["weather"][0]["description"].asString();
        QString weatherDescriptionQStr = QString::fromUtf8(weatherDescription.c_str());

        setStatus(weatherDescriptionQStr);
        setAirPressure(airPressureLevel);
        setWindSpeed(windSpeedkmh);
        setTemperature(temperatureCelsius);
        setHumidity(humidityLevel);
        setUvIndex(uvIndexLevel);

        reformatStatusText();
        getSuitableIconFromStatus();

        // Clean up
        curl_easy_cleanup(curl);
    }
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
            qDebug() << resultStringStatus[i+1];
        }
    }
    setStatus(resultStringStatus);
}

void Weather::getSuitableIconFromStatus()
{
    QString loweredStatusString = status().toLower();

    if (loweredStatusString == "clear sky") {
        setStatusIconPath("sunny");
    }

    if (loweredStatusString.contains("rain")) // light or heavy rain
        setStatusIconPath("rainy");

    else if (loweredStatusString == "broken clouds" ||
             loweredStatusString == "few clouds" ||
             loweredStatusString == "scattered clouds")
        setStatusIconPath("few-clouds");

    else if (loweredStatusString.contains("snow"))
        setStatusIconPath("snowy");

    else if (loweredStatusString.contains("clouds"))
        setStatusIconPath("cloudy");
}

size_t Weather::WriteCallbackAPI(void *contents, size_t size,
                                 size_t nmemb, std::string *data)
{
    size_t totalSize = size * nmemb;
    data->append(static_cast<char*>(contents), totalSize);
    return totalSize;
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

const QString &Weather::statusIconPath() const
{
    return m_statusIconPath;
}

void Weather::setStatusIconPath(const QString &newStatusIconPath)
{
    if (m_statusIconPath == newStatusIconPath)
        return;
    m_statusIconPath = "icons/" + newStatusIconPath + ".png";
    emit statusIconPathChanged();
}
