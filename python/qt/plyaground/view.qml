import QtQuick 1.0

Rectangle {
  id: page

  signal textRotationChanged(double root)

  width: 500; height: 200; color: "lightgray"

  Text {
    text: "Hello Berm"
    anchors.centerIn: parent
    font.pixelSize: 24
    color: 'black'

    onRotationChanged: textRotationChanged(rotation)

    MouseArea {
      anchors.fill: parent
      onClicked: {
        console.log("Mouse clicked!", parent.color)
        con.outputStr('Hi')
        if (parent.color == "#000000")
          parent.color = 'blue';
        else
          parent.color = 'black';                
        parent.rotation = rotatevalue.val();
      }
    }
  }
}

