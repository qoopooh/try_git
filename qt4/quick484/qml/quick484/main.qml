import QtQuick 1.1

Rectangle {
    width: 100
    height: 100
    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            myObject.cppMethod("Hello from QML")
            myObject.cppSlot(12345)
//            Qt.quit();
        }
    }
}
