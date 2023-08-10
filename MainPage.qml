import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import com.mycompany.weather 1.0
import com.mycompany.datetime 1.0
import com.mycompany.task 1.0
import com.mycompany.weaminder 1.0

Item {
    Weaminder {
        id: weaminder
    }

    DateTime {
        id: dateTime
    }

    TasksLoader {
        id: tasksLoader

    }

    Weather {
        id: weather
        tasksLoader: tasksLoader

        onWeatherPropertiesChanged: {
            taskListView.reloadTasks()
            weatherTimeSelector.addTimeToList()
            weatherDaySelector.dailyWeatherIconUrlList = weather.getDailyWeatherIconUrlsListFromJsonData()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8

        RowLayout {
            Layout.fillWidth: true
            spacing: 2

            CitySelector {
                id: citySelector
                implicitWidth: 220
                Layout.fillWidth: true
            }

            WeatherTimeSelector {
                id: weatherTimeSelector
                implicitWidth: 120
                Layout.fillWidth: true
            }
        }

        WeatherInfo {
            id: weatherInfoWidget

            Layout.fillWidth: true

            weatherStatus: weather.status
            weatherStatusIconPath: weather.statusIconUrl
            temperature: weather.temperature
            humidity: weather.humidity
            windSpeed: weather.windSpeed
            uvIndex: weather.uvIndex
            rain: weather.rain
        }

        Label {
            id: rememberTitle

            text: "Remember to:"
            font.pointSize: 13
        }

        TasksListView {
            id: taskListView
            Layout.fillWidth: true
            Layout.fillHeight: true

        }
    }

    SelectDayButton {
        id: selectDayButton

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 8
    }

    WeatherDaySelector {
        id: weatherDaySelector

        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -128
    }

    EasterEgg {
        anchors.fill: parent
    }

    WifiOffNotification {
        id: wifiOffNotification
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
    }
}
