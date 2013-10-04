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

  Rectangle {
    id: rotater
    width: 60; height: 40; color: 'darkgray';
    x: 60; y: 60;
       //anchors.bottom: page.bottom;
    property real angle: 0
    MouseArea {
      id: 'btnMouseArea'
      objectName: 'btnMouseArea'
      anchors.fill: parent
    }
    Text {
      id: btnText
      text: 'Press me'
      anchors.centerIn: parent
    }

    transform: Rotation {
      origin.x: 10; origin.y: 5
      angle: rotater.angle
      Behavior on angle {
        SpringAnimation {
          spring: 1.4
          damping: .05
        }
      }
    }
  }

  function updateRotater() {
    rotater.angle = rotater.angle + 45
  }
}

