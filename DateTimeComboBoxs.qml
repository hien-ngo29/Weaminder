import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    ComboBox {
        id: dateComboBox

        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        model: [ "Monday", "Tuesday", "Wednesday",
            "Thursday", "Friday", "Saturday", "Sunday" ]
    }

    ComboBox {
        id: timeComboBox

    }
}
