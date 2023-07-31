import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: column

    property var dailyWeatherIconUrlList;

    MouseArea {
        id: closeArea
        width: 10; height: 498

        Layout.fillHeight: true
        Layout.fillWidth: true

        visible: false

        onClicked: {
            weatherDaySelector.state = "hidden"
            selectDayButton.state = "showed"
        }

    }

    onStateChanged: {
        if (state == "showed")
            closeArea.visible = true
        else
            closeArea.visible = false
    }

    Component.onCompleted: state = "hidden"

    Flickable {
        Layout.fillWidth: true

        height: 128
        contentWidth: 928; contentHeight: 128

        flickableDirection: Flickable.HorizontalFlick

        RowLayout {
            anchors.fill: parent
            Repeater {
                model: 7
                property int index;
                WeatherDayWidget {
                    dayOfWeek: dateTime.getFollowingDaysInWeekFromToday()[index]
                    isCurrentDay: index == 0
                    weatherIconUrl: dailyWeatherIconUrlList[index]
                }
            }
        }
    }

    states: [
        State {
            name: "showed"; when: selectDayButton.clicked
            PropertyChanges { target: weatherDaySelector; anchors.bottomMargin: 9 }
        },
        State {
            name: "hidden";
            PropertyChanges { target: weatherDaySelector; anchors.bottomMargin: -128 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "anchors.bottomMargin"; duration: 500; easing.type: Easing.InOutQuad }
    }
}
