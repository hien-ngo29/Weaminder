import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ComboBox {
    id: weatherTimeSelector

    model: ListModel {
        id: listModel
    }

    delegate: Item {
        width: parent.width
        height: 50

        Rectangle {
            id: background
            anchors.fill: parent
            color: (isHighlighted === false) ? "#949494" : "#bababa"
        }

        Row {
            spacing: 10

            anchors.verticalCenter: parent.verticalCenter

            Image {
                source: modelIcon
                width: 50
                height: 50
                anchors.margins: 8
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                font.pointSize: 12
                text: modelData // Display the item text
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                listModel.get(currentIndex).isHighlighted = false
                currentIndex = index
                listModel.get(index).isHighlighted = true

                weather.currentHourInDay = index
                weather.reloadWeatherFromLocation(citySelector.currentText)

                weatherTimeSelector.popup.close()
            }
        }
    }

    function addTimeToList() {
        listModel.clear()
        currentIndex = weather.currentHourInDay
        for (var i = 0; i < 24; i++) {
            listModel.append({
                modelData: dateTime.getFollowingHours()[i],
                modelIcon: weather.weatherIconPaths[i],
                isHighlighted: currentIndex === i
            });
        }
    }
}
