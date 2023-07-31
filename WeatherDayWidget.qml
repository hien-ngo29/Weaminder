import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    property string dayOfWeek;
    property string iconUrl;
    property bool isCurrentDay;
    property string weatherIconUrl;

    property int dayIndex;

    opacity: 0.7
    radius: 18

    width: 128
    height: 128

    Rectangle {
        id: backgroundHighlighter
        anchors.fill: parent
        color: "#919191"
        visible: isCurrentDay
        radius: 18
    }

    Column {
        anchors.fill: parent

        Image {
            width: 96
            height: 96
            scale: 1.3
            source: weatherIconUrl
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            width: 110; height: 50
            text: dayOfWeek
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            font.pointSize: 15
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        id: selectArea
        anchors.fill: parent

        onClicked: {
            weatherDaySelector.setStateToHidden()
            weather.currentDay = dayIndex
            weatherDaySelector.unhighlightAllWeatherDayWidgets()
            isCurrentDay = true
            weather.reloadWeatherFromLocation(citySelector.currentText)
        }
    }

    color: "#828282"
}
