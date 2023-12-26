import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "#4a4a4a"
    radius: 10
    width: 250; height: 50

    state: "hidden"

    Component.onCompleted: checkConnection()

    Row {
        anchors.centerIn: parent
        spacing: 10
        Image {
            width: 32; height: 32
            source: "icons/connection-off.png"
        }

        Label {
            text: "You are currently offline"
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }
    }

    Timer {
        id: closeTimer
        interval: 2000
        running: false

        onTriggered: {
            state = "hidden"
            closeTimer.restart()
            closeTimer.running = false
        }
    }

    states: [
        State {
            name: "showed";
            PropertyChanges { target: offlineNotification; anchors.topMargin: 9 }
        },
        State {
            name: "hidden";
            PropertyChanges { target: offlineNotification; anchors.topMargin: -50 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "anchors.topMargin"; duration: 500; easing.type: Easing.InOutQuad }
    }

    function showNotification() {
        state = "showed"
        closeTimer.running = true
    }

    function checkConnection() {
        if (!weaminder.isConnectionOn()) showNotification()
    }
}
