import QtQuick 1.0

Rectangle {
  width: 200
  height: 200
  color: "white"

  Text {
    text: "Hello Berm"
    anchors.centerIn: parent
    font.pixelSize: 24
    color: 'black'

    MouseArea {
      anchors.fill: parent
      onClicked: {
        console.log("Mouse clicked!", parent.color)
        con.outputStr('Hi')
        if (parent.color == "#000000")
          parent.color = 'blue';
        else
          parent.color = 'black';                
      }
    }
  }
}

