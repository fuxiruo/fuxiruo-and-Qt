import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "Snake.js" as Snake

Window{
    id:root
    title:"snake"

    width: play.width+menu.width+info.width
    height: 400
    maximumWidth: width
    maximumHeight: height

    property bool bStart: false
    property bool bPause: false

    onActiveFocusItemChanged: {
//        console.log(activeFocusItem)
    }

    Rectangle{
        id:menu
        width: layout_btn.width; height: parent.height
        anchors { top: parent.top; left: parent.left }

        color: "gray"

        ColumnLayout{
            id:layout_btn
            width: 140;
            anchors{
                horizontalCenter: parent.horizontalCenter;
                verticalCenter: parent.verticalCenter}
            spacing: 10

            Button {
                id:btn_new
                text: "New Game"

                font.family: "微软雅黑"
                font.pointSize: 12

                background: Rectangle {
                    opacity: enabled ? 1 : 0.3
                    color: btn_new.down ? "#827eb1" : "gray"
                    border.width: 1
                    radius: 2
                }

                anchors{
                    left: parent.left;
                    right: parent.right;
                    leftMargin: 10;
                    rightMargin: 10
                }

                onClicked: {
                    if(!bStart){
                        bStart = true
                        bPause = false
                        play.focus = true
                        text = "Pause"
                        Snake.newGame();
                        timer_produce_food.start();
                        text_game_over.visible = false
                    }else{
                        if(bPause){
                            bPause = false
                            play.focus = true
                            timer_produce_food.stop();
                            text = "Pause"
                        }else{
                            bPause = true
                            timer_produce_food.start();
                            text = "Continue"
                        }
                    }
                }
            }

            Button {
                id:btn_quit
                text: "Quit Game"

                font.family: "微软雅黑"
                font.pointSize: 12

                background: Rectangle {
                    opacity: enabled ? 1 : 0.3
                    color: btn_quit.down ? "#827eb1" : "gray"
                    border.width: 1
                    radius: 2
                }

                anchors{
                    left: parent.left;
                    right: parent.right;
                    leftMargin: 10;
                    rightMargin: 10
                }

                onClicked: {
                    Qt.quit();
                }
            }
        }
    }

    Rectangle{
        id:play

        property int level: 0
        property int score: 0

        width: 300; height: parent.height
        anchors { left: menu.right}

        color: "black"
        focus: false

        Text {
            id: text_game_over
            visible: false;
            anchors{
                horizontalCenter: parent.horizontalCenter;
                verticalCenter: parent.verticalCenter}

            color: "red"
            text: "Game Over"
            font.family: "微软雅黑"
            font.pointSize: 20

            SequentialAnimation {
                running: text_game_over.visible
                loops: Animation.Infinite
                NumberAnimation {
                    target: text_game_over
                    property: "opacity"
                    from: 1
                    to: 0.1
                    duration: 1500
                    easing.type: Easing.Linear
                }
                NumberAnimation {
                    target: text_game_over
                    property: "opacity"
                    from: 0.1
                    to: 1
                    duration: 1500
                    easing.type: Easing.Linear
                }
            }
        }

        Keys.onPressed: {
            Snake.changeDir(event.key);
        }
    }


    Rectangle{
        id:info
        width: 100; height: parent.height
        anchors { left: play.right }

        color: "gray"

        Item {
            id: item_texts
            anchors{
                horizontalCenter: parent.horizontalCenter;
                verticalCenter: parent.verticalCenter}

            Text {
                id: text_level
                anchors{
                    horizontalCenter: parent.horizontalCenter}

                text: "Level:" + play.level
                font.family: "微软雅黑"
                font.pointSize: 14
            }

            Text {
                id: text_score
                anchors{
                    horizontalCenter: parent.horizontalCenter;
                    top: text_level.bottom
                }

                text: "Score:" + play.score
                font.family: "微软雅黑"
                font.pointSize: 14
            }
        }
    }

    Timer{
        id:timer_produce_food
        interval: 800;
        repeat: true;
        onTriggered: {
//            console.debug('on timer');
            Snake.foodProducer();
            Snake.autoMove();

            if(Snake.checkGameOver()){
                stop();
                root.bStart = false;
                btn_new.text = "New Game"
                text_game_over.visible = true
            }
        }
    }
}
