import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    property string taskName

    Label {
        text: taskName
        font.pointSize: 13
    }

    Item {
        Layout.fillWidth: true
    }

    CheckBox {

    }
}
