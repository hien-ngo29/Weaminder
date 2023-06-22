import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


ComboBox {
    anchors.right: parent.right
    anchors.left: parent.left

    model: [
        "Toronto, Canada",
        "Montreal, Canada",
        "Vancouver, Canada",
        "Calgary, Canada",
        "Austin, USA",
        "Boston, USA",
        "Chicago, USA",
        "Houston, USA",
        "Florida, USA",
        "New York, USA",
        "Hawaii, USA",
        "Michigan, USA",
        "Pennsylvania, USA",
        "Kentucky, USA",
        "Louisiana, USA",
        "California, USA",
        "Washington DC, USA",
        "Texas, USA",
        "Ho Chi Minh, Vietnam",
        "Phan Thiet, Vietnam",
        "Long Khanh, Vietnam",
        "Ha Noi, Vietnam",
        "Thua Thien Hue, Vietnam",
        "Vung Tau, Vietnam",
        "Da Lat, Vietnam",
        "Da Nang, Vietnam",
        "Nha Trang, Vietnam"
    ]

    currentIndex: 17 // Texas, USA

    onCurrentIndexChanged: {
        weather.reloadWeatherFromLocation(model[currentIndex])
        taskListView.reloadTasks()
    }
}
