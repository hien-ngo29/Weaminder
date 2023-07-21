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

        onTemperatureChanged: {
            taskListView.reloadTasks()
            weatherTimeSelector.addTimeToList()
        }
    }

    Weather {
        id: weather
        tasksLoader: tasksLoader
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
            }

            WeatherTimeSelector {
                id: weatherTimeSelector
                implicitWidth: 120
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
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 8
    }

    EasterEgg {
        anchors.fill: parent
    }
}
