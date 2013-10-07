import Qt 4.7

Rectangle {
    id: main
    color: "#343434"
    width: 200
    height: 200

    Component {
        id: listDelegate

        Item {
            id: listItem
            property string text
            anchors.right: parent.right
            anchors.left: parent.left
            height: 40
    
            Rectangle {
                id: frame
                anchors.fill: parent
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                color: "#00000000"
                radius: 20
                border.width: 2
                border.color: "#ffffff"
    
    
                Text {
                    id: itemText
                    color: "#ffffff"
                    text: model.cars.name  // Here we get the value from the model, it's column and it's item 
                    anchors.fill: parent
                    anchors.rightMargin: 5
                    anchors.leftMargin: 10
                    anchors.bottomMargin: 5
                    anchors.topMargin: 5
                    clip: false
                    style: Text.Normal
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    smooth: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.pointSize: 14
                }
            }
    
            MouseArea {
                 anchors.fill: parent
                 onClicked: {
                     listView.currentIndex = index
                     controller.itemSelected(model.cars, listModel) // We call the Python function with 2 parameters
                 }
            }
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: listModel // We assign here the model created by PySide script
        delegate: listDelegate
        focus: true
        clip: true
    }
}
