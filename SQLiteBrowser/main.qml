import QtQuick 2.9
import QtQuick.Window 2.2
//import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.2

import ko.sql.model 1.0
import "utils" as Utils
import "views" as Views

Views.AppWindow {
    id: superRoot

    SqlModel{
        id: _SQLite
    }

    onActiveDatabaseChanged: {
        _SQLite.qmlOpenDatabase(activeDatabase)
        _ListView_Tables.model = _SQLite.qmlGetTables()
    }

    ColumnLayout{
        id: superLayout
        anchors.fill: parent
        spacing: 0

        //GUI
        Views.Header {
            id: _Header
            z: theme.z.header
        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: _Rectangle_Navigation

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                //                Layout.fillHeight: true

                width: 72
                color: theme.asphalt

                ListView {
                    id: _ListView_Navigation

                    anchors.fill: parent

                    interactive: false

                    model: [
                        {
                            image : "img/icon-tables.png"
                        }
                    ]

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: width
                        color: ListView.view.currentIndex === index ? Qt.darker(theme.asphalt) : theme.asphalt
                        Utils.BaseIcon {
                            anchors.centerIn: parent
                            width: 32
                            source: modelData.image
                            color: theme.lightblue
                        }
                    }
                }
            }

            Rectangle {
                id: _Rectangle_Tables

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: _Rectangle_Navigation.right

                //                Layout.fillHeight: true

                width: 150
                color: theme.white

                Utils.AccentRight {
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.SizeHorCursor
                        property int _x : 0
                        onPressed: {
                            _x = mouse.x
                        }
                        onPositionChanged: {
                            _Rectangle_Tables.width = Math.max(150, _Rectangle_Tables.width + mouse.x - _x)
                        }
                    }
                    color: "transparent"
                    width: 4
                    z: 3
                }

                Utils.Blurtangle {
                    id: _Item_TablesHeader
                    anchors.top: parent.top
                    height: 30
                    width: parent.width
                    attachTo: _ListView_Tables
                    color: "#ffffff"

                    Utils.AccentBottom {//下划线
                        color: theme.headerAccent
                    }

                    Views.Label {
                        anchors.fill: parent
                        anchors.margins: 8
                        text: qsTr("Tables")
                        font.capitalization: Font.AllUppercase
                        font.bold: true
                        font.pixelSize: 12
                    }
                    z: 2
                }

                ListView {
                    id: _ListView_Tables

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    currentIndex: -1
                    z: 1

                    onCurrentIndexChanged: {
                        _TableView.resetTableHeaders(currentItem.dataModel.modelData)
                    }

                    header: Item {
                        width: 1
                        height: _Item_TablesHeader.height
                    }

                    model: _SQLite.qmlGetTables()

                    delegate: Rectangle {
                        property variant dataModel : model || {}//currentItem可以直接访问model,外部访问不了,需要通过属性dataModel暴露出来
                        width: ListView.view.width
                        height: 30
                        color: ListView.view.currentIndex === index ? theme.dirtywhite : "transparent"
                        Row {
                            anchors.left: parent.left
                            anchors.leftMargin: 15
                            anchors.right: parent.left
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 6
                            Rectangle {
                                width: 12
                                height: 12
                                radius: 6
                                color: theme.blue
                                anchors.verticalCenter: _Text_TableName.verticalCenter
                            }
                            Views.Label {
                                id: _Text_TableName
                                height: 20
                                /*Models that do not have named roles
(such as the ListModel shown below)
will have the data provided via the modelData role.
The modelData role is also provided for models that have only one role.
In this case the modelData role contains the same data as the named role.*/
                                text: model.modelData
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                parent.ListView.view.currentIndex = index
                            }
                        }
                    }
                }
            }

            Item {
                id: _Item_Container

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: _Rectangle_Tables.right
                anchors.right: parent.right

                //                Layout.fillWidth: true
                //                Layout.fillHeight: true

                TableView {
                    id: _TableView
                    property string tableViewColumnBuilder : "import QtQuick 2.9; import QtQuick.Controls 1.2; import QtQuick.Layouts 1.2; TableViewColumn { role: \"%roleName%\"; title: role; width: 100 }"
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    itemDelegate: Item {
                        width: 100
                        clip: true

                        Utils.AccentRight {
                            color: theme.headerAccent
                            opacity: 0.3
                        }

                        Views.Label {
                            id: _Text
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            color: styleData.textColor
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            width: parent.width
                            text: String(styleData.value)
                        }
                    }

                    function resetTableHeaders(tableName){
                        _SQLite.qmlSetTableName(tableName)

                        for(var i = columnCount - 1; i >= 0; i--)
                        {
                            removeColumn(i)
                        }
                        var headers = _SQLite.qmlGetHeader();
                        //console.debug(headers)
                        for(var j in headers){
                            var c = Qt.createQmlObject(tableViewColumnBuilder.replace(/%roleName%/g, headers[j]), _TableView, "")
                            addColumn(c)
                        }

                        var result = _SQLite.qmlGetResult()
                        if(result.length === 0)
                        {
                            _TableView.model = []
                            return
                        }
                        _TableView.model = result
                        _TableView.resizeColumnsToContents()
                    }
                }
            }
        }
    }
}

