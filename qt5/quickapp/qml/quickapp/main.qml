import QtQuick 2.0

Rectangle {
    id: main
    width: 360
    height: 360

    Rectangle {
        id: rectangle1
        x: 80
        y: 200
        width: 200
        height: 100
        color: "blue"
        rotation: 30
        anchors.verticalCenter: parent.verticalCenter
//        opacity: 90

        Text {
            id: text1
            text: qsTr("Some Text")
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }
    Image {
        source: "http://www.google.com/images/srpr/logo3w.png"
        rotation: 5
        opacity: 20
        x: 10
        y: 20
        width: 90
        height: 50
    }

}
