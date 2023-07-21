import QtQuick 2.0
import QtQuick.Controls 2.15

RoundButton {
    id: selectDayButton

    width: 80; height: 80

    Component.onCompleted: state = "show"

    onClicked: state = "hidden"

    Image {
        width: 48; height: 48
        source: "icons/timetable.png"
        anchors.centerIn: parent
    }

    states: [
        State {
            name: "showed"; when: selectDayButton.clicked
            PropertyChanges { target: selectDayButton; anchors.rightMargin: 8 }
        },
        State {
            name: "hidden"; when: 1 == 2
            PropertyChanges { target: selectDayButton; anchors.rightMargin: -80 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "anchors.rightMargin"; duration: 500; easing.type: Easing.InOutQuad }
    }
}
