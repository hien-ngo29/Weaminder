import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Column {
    spacing: 2

    Image {
        id: weatherStatusIcon
        width: 100; height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        source: weather.statusIconUrl
        scale: 1.8
    }

    Label {
        id: temperatureLabel
        anchors.horizontalCenter: parent.horizontalCenter
        text: weather.temperature + "°C"
        font.pointSize: 42
    }

    Label {
        id: statusLabel
        text: "Status: " + weather.status
        font.pointSize: 14
    }

    Label {
        id: humidityLabel
        text: "Humidity: " + weather.humidity + " %"
        font.pointSize: 13
        font.weight: Font.Light
    }

    Label {
        id: windSpeedLabel
        text: "Wind: " + weather.windSpeed + " Km/h"
        font.pointSize: 13
        font.weight: Font.Light
    }

    Label {
        id: rainLabel
        text: "Rain: "  + weather.rain + " mm"
        font.pointSize: 13
        font.weight: Font.Light
    }

    Label {
        id: uvIndexLabel
        text: "UV: " + weather.uvIndex
        font.pointSize: 13
        font.weight: Font.Light
    }
}
