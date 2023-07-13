import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Column {
    spacing: 2

    property string weatherStatus
    property string weatherStatusIconPath
    property double temperature
    property double humidity
    property double windSpeed
    property int airPressure
    property int uvIndex

    Image {
        id: weatherStatusIcon
        width: 100; height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        source: weatherStatusIconPath
        scale: 1.8
    }

    Label {
        id: temperatureLabel
        anchors.horizontalCenter: parent.horizontalCenter
        text: temperature + "°C"
        font.pointSize: 42
    }

    Label {
        id: statusLabel
        text: "Status: " + weatherStatus
        font.pointSize: 14
        font.weight: Font.bold
    }

    Label {
        id: humidityLabel
        text: "Humidity: " + humidity + " %"
        font.pointSize: 13
        font.weight: Font.Light
    }

    Label {
        id: windSpeedLabel
        text: "Wind: " + windSpeed + " Km/h"
        font.pointSize: 13
        font.weight: Font.Light
    }

    Label {
        id: airPressureLabel
        text: "Air Pressure: "  + airPressure + " hPa"
        font.pointSize: 13
        font.weight: Font.Light
    }

    Label {
        id: uvIndexLabel
        text: "UV: " + uvIndex
        font.pointSize: 13
        font.weight: Font.Light
    }
}
