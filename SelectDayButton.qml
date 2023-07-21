import QtQuick 2.0
import QtQuick.Controls 2.15

RoundButton {
    id: selectDayButton

    width: 80; height: 80

    Image {
        width: 48; height: 48
        source: "icons/timetable.png"
        anchors.centerIn: parent
    }
}
