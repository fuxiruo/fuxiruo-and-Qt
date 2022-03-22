import QtQuick 2.0

Item {
    property alias color: rect.color
    width: 16
    height: 16

    Rectangle{
        id:rect

        color: "green"
        anchors.fill: parent
    }
}
