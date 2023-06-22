import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ItemDelegate {
    width: parent.width
    height: 60

    contentItem: Task {
        taskName: model.taskName
    }
}
