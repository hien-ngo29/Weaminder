import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ComboBox {

    anchors.right: parent.right
    anchors.left: parent.left

    model: ListModel {
        id: listModel
    }

    function addTimeToList() {
        for (let index in weaminder.getFollowingHours())
            listModel.append({text :
                                weaminder.getFollowingHours()[index]});
    }

    Component.onCompleted: addTimeToList()
}
