import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    anchors.left: parent.left
    anchors.right: parent.right

    height: 40

    gradient: theme.headerGradient
    radius: theme.windowRadius

    MouseArea {
        property int _x
        property int _y

        anchors.fill: parent

        onPressed: {
            _x = mouse.x
            _y = mouse.y
        }
        onPositionChanged: {
            superRoot.x = superRoot.x + mouse.x - _x
            superRoot.y = superRoot.y + mouse.y - _y
        }
    }

    Label {
        anchors.centerIn: parent

        width: 500

        horizontalAlignment: Text.AlignHCenter
        color: theme.text
        styleColor: theme.white
        style: Text.Raised
        text: activeDatabase.toString()
    }

    Row {
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        layoutDirection: Qt.RightToLeft
        spacing: 10

        Button {
            height: 30
            text: "Open Database"
            onClicked: {
                 fileDialog.open()
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 1
        color: theme.headerAccent
    }
}
