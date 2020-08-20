import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 600
    height: 400
    title: qsTr("Hello World")

    Clock{
        id: clock1
        width: Math.min(parent.width, parent.height)
        height: width
        anchors.centerIn: parent
    }

    MouseArea{
        anchors.fill: parent
        id:mouseArea
        onPressed:  {
            console.log(mouseX, mouseY)
        }
    }
}
