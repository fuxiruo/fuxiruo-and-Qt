import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("SQLite Browser")

    property alias theme : _Theme
    property alias fileDialog : _FileDialog
    property string activeDatabase: ""
    FileDialog {
        id: _FileDialog
        title: "Please choose a file"
        nameFilters: [ "SQLite3 Databases (*.db *.sqlite3 *.sqlite *.sql3)", "All files (*)" ]
        onAccepted: {
            activeDatabase = fileUrl
        }
    }

    Theme {
        id: _Theme
    }
}
