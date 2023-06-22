import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import com.mycompany.weather 1.0
import com.mycompany.task 1.0

Item {
    Weather {
        id: weather
    }

    TasksLoader {
        id: tasksLoader
        currentWeather: weather
    }

    CitySelector {
        id: citySelector

        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        anchors.margins: 8
    }

    WeatherInfo {
        id: weatherInfoWidget
        anchors.margins: 8
        anchors.right: parent.right
        anchors.left: parent.left

        anchors.top: citySelector.bottom

        weatherStatus: weather.status
        weatherStatusIconPath: weather.statusIconPath
        temperature: weather.temperature
        humidity: weather.humidity
        windSpeed: weather.windSpeed
        uvIndex: weather.uvIndex
        airPressure: weather.airPressure
    }

    Label {
        id: rememberTitle
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: weatherInfoWidget.bottom
        anchors.margins: 8
        anchors.topMargin: 12

        text: "Remember to:"
        font.pointSize: 13
    }

    TasksListView {
        id: taskListView
        anchors.margins: 8
        anchors.top: rememberTitle.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
}
