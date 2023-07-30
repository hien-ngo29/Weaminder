import QtQuick 2.0
import QtQuick.Controls 2.15

RoundButton {
    width: 80; height: 80

    state: "showed"

    onClicked: {
        state = "hidden"
        weatherDaySelector.state = "showed"
    }

    Image {
        width: 48; height: 48
        source: "icons/timetable.png"
        anchors.centerIn: parent
    }

    states: [
        State {
            name: "showed"; when: true
            PropertyChanges { target: selectDayButton; anchors.rightMargin: 8 }
        },
        State {
            name: "hidden"; when: selectDayButton.clicked
            PropertyChanges { target: selectDayButton; anchors.rightMargin: -96 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "anchors.rightMargin"; duration: 500; easing.type: Easing.InOutQuad }
    }
}
