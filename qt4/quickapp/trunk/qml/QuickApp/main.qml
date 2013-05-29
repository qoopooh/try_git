import QtQuick 1.0

Rectangle {
    width: 360
    height: 360
    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            item.qmlSignal("Hello from QML");
            Qt.quit();
        }
    }
}
