import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15


Frame {

    ListView {
        anchors.fill: parent

        id: tasksList

        layer.enabled: true
        delegate: TasksListDelegate { }
        model: ListModel {
            id: tasksListModel
        }

        Component.onCompleted: {
            reloadTasks()
        }
    }

    Label {
        text: "There is nothing to do :)"
        anchors.centerIn: parent
        font.pointSize: 16
        color: "gray"

        visible: tasksList.count == 0
    }

    function reloadTasks() {
        tasksListModel.clear()
        for (let index in tasksLoader.getSuitableTasksWithWeather())
            tasksListModel.append({"taskName" :
                                tasksLoader.getSuitableTasksWithWeather()[index]});
    }
}
