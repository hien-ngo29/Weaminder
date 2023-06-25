import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    Rectangle {
        id: background
        color: "black"


        anchors.fill: parent

        Column {
            anchors.centerIn: parent
            Label {
                text: "Have a nice day <3"
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: "\n😊"
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        opacity: 0

        OpacityAnimator {
            id: fadeInAnimation
            target: background;
            from: 0;
            to: 1;
            duration: 1000
            running: false
        }

        OpacityAnimator {
            id: fadeOutAnimation
            target: background;
            from: 1;
            to: 0;
            duration: 1000
            running: false
        }
    }

    Button {
        text: "Click me!"

        width: 20

        anchors.verticalCenter: parent.verticalCenter

        anchors.left: parent.right
        anchors.leftMargin: -12

        onClicked:  {
            if (text == "Click me!") {
                text = "😊"
                fadeInAnimation.running = true
            }
            else if (text == "😊") {
                text = "Click me!"
                fadeOutAnimation.running = true
            }
        }
    }
}
