import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Window {
    id: superRoot

    visible: true
    width: 640
    height: 480
    title: qsTr("Puzzle")

    property string borderColor: "green"

    property string activeImg: ""
    readonly property int defaultSize: 80
    readonly property real surfaceViewportRatio: 1.0

    Component.onCompleted: {
        var r = 3
        var c = 3

        for(var i=0; i<9; i++){
            var o = new Object
            o.left = (i%c)?(i-1):-1;
            o.right = (i+1)%c?(i+1):-1;
            o.top = (i-c)?i-c:-1;
            o.bottom = (i+c)<r*c?i+c:-1;
            //console.debug(i,o.left, o.right, o.top, o.bottom)
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a imgage"
        nameFilters: [ "imgage (*.png *.jpeg *.bmp)", "All files (*)" ]
        onAccepted: {
            text_finish.visible = false
            rep.model = 0
            activeImg = fileUrl.toString()
            if(activeImg.length > 0){
                rep.model = 4
            }
        }
    }

    Column{
        id: colRoot
        anchors.fill: parent
        anchors.margins: 2

        //header
        Rectangle{
            id: rectHeader

            anchors.left: parent.left
            anchors.right: parent.right

            border.color: borderColor
            border.width: 2

            height: 40

            Label {
                anchors.centerIn: parent

                horizontalAlignment: Text.AlignHCenter
                text: activeImg
            }

            Row {
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                layoutDirection: Qt.RightToLeft
                spacing: 10

                Button {
                    height: 30
                    text: "Open Image"
                    onClicked: {
                        fileDialog.open()
                    }
                }
            }
        }

        //worker
        Row{
            anchors.left: parent.left
            anchors.right: parent.right

            //status
            Rectangle{
                id: rectStatus

                border.color: borderColor
                border.width: 2

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: border.width

                width: 100

                Image{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    source: activeImg
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: parent.width - parent.border.width
                    sourceSize.height: parent.width - parent.border.width
                }
            }

            Rectangle{
                width: parent.width - rectStatus.width
                height: superRoot.height-rectHeader.height

                //puzzle
                Flickable {
                    id: flicImg

                    width: parent.width
                    height: parent.height
                    contentWidth: width * surfaceViewportRatio
                    contentHeight: height * surfaceViewportRatio

                    property var currentFrame: undefined
                    property int highestZ: 0

                    Repeater {
                        id: rep
                        model: 0

                        property int rowCount: Math.sqrt(rep.count)
                        property int colCount: Math.sqrt(rep.count)

                        Rectangle {
                            id: photoFrame

                            property int no: index
                            property real prevX: 0
                            property real prevY: 0
                            property var topBottomLeftRightIndex: new Object

                            width: image.width
                            height: image.height

                            Behavior on scale { NumberAnimation { duration: 200 } }
                            Behavior on x { NumberAnimation { duration: 200 } }
                            Behavior on y { NumberAnimation { duration: 200 } }
                            border.color: "red"
                            border.width: 2
                            smooth: true
                            antialiasing: true

                            Component.onCompleted: {
                                x = Math.random() * (flicImg.width-defaultSize)
                                y = Math.random() * (flicImg.height-defaultSize)

                                topBottomLeftRightIndex.left = -1
                                topBottomLeftRightIndex.right = -1
                                topBottomLeftRightIndex.top = -1
                                topBottomLeftRightIndex.bottom = -1
                            }

                            Image {
                                id: image
                                anchors.centerIn: parent
                                fillMode: Image.PreserveAspectFit
                                sourceSize.width: defaultSize
                                sourceSize.height: defaultSize
                                source: "image://puzzleImage/" + rep.count + "#" + index + "#" + activeImg
                                antialiasing: true
                            }

                            MouseArea {
                                id: dragArea
                                anchors.fill: parent
                                drag.target: photoFrame
                                onPressed: {
                                    //console.debug(parent,parent.x,parent.y)
                                    photoFrame.prevX = photoFrame.x
                                    photoFrame.prevY = photoFrame.y

                                    photoFrame.z = ++flicImg.highestZ;
                                    setFrameColor(true);
                                }
                                onReleased: {
                                    //console.debug(no,parent.x,parent.y)
                                    setFrameColor(false);
                                    flicImg.checkPos(no);
                                }
                            }

                            function setFrameColor(bPressed) {
//                                if (flicImg.currentFrame){
//                                    flicImg.currentFrame.border.color = "red";
//                                }
                                if(!bPressed){
                                    flicImg.currentFrame = undefined
                                    return;
                                }
                                flicImg.currentFrame = photoFrame;
                                flicImg.currentFrame.border.color = "green";
                            }

                            function checkIfTopBottomLeftRightOK(){
                                var o = new Object
                                o.left = (no%rep.colCount)?(no-1):-1;
                                o.right = (no+1)%rep.colCount?(no+1):-1;
                                o.top = (no-rep.colCount>=0)?no-rep.colCount:-1;
                                o.bottom = (no+rep.colCount)<rep.count?no+rep.colCount:-1;
                                console.debug(no,o.left, o.right, o.top, o.bottom)
                                console.debug(no,topBottomLeftRightIndex.left, topBottomLeftRightIndex.right, topBottomLeftRightIndex.top, topBottomLeftRightIndex.bottom)

                                if(o.left == topBottomLeftRightIndex.left &&
                                        o.right == topBottomLeftRightIndex.right &&
                                        o.top == topBottomLeftRightIndex.top &&
                                        o.bottom == topBottomLeftRightIndex.bottom){

                                    border.color = "transparent";

                                    return true;
                                }else{
                                    return false;
                                }
                            }
                        }
                    }

                    function checkPos(moveNo){
                        /*
                      Items instantiated by the Repeater are inserted, in order, as children of the Repeater's parent
                    Items declared as children of a Flickable are automatically parented to the Flickable's contentItem
                    */
                        //flicImg.contentItem包含Repeater本身和Repeater创建出来的Item
                        var bFindIntersected = false
                        var itemMove = flicImg.contentItem.children[moveNo]
                        var vaildGap = defaultSize/4
                        for(var i=0; i<flicImg.contentItem.children.length; i++){
                            var puzzleItem = flicImg.contentItem.children[i]
                            if(puzzleItem===rep){//不比较Repeater
                                console.debug("skip Repeater")
                                continue
                            }
                            if(i==moveNo){
                                console.debug("skip self",puzzleItem.x,puzzleItem.y)
                                continue
                            }
                            //                        console.debug(i,puzzleItem.x,puzzleItem.y,puzzleItem.width,puzzleItem.height)
                            //                        console.debug(i,x>=puzzleItem.x-w,x<=puzzleItem.x+puzzleItem.width,y>=puzzleItem.y-h,y<=puzzleItem.y+puzzleItem.height,
                            //                                      puzzleItem.x-w,puzzleItem.x+puzzleItem.width,puzzleItem.y-h,puzzleItem.y+puzzleItem.height)
                            //边界重合比较
                            if(itemMove.x>=puzzleItem.x-itemMove.width && itemMove.x<=puzzleItem.x+puzzleItem.width){
                                if(itemMove.y>=puzzleItem.y-itemMove.height && itemMove.y<=puzzleItem.y+puzzleItem.height){
                                    console.debug(moveNo,"intersected",i,Math.abs(itemMove.x-puzzleItem.x), Math.abs(itemMove.y-puzzleItem.y))

                                    //边界有重合，查看是否足够靠近就自动贴合
                                    if(Math.abs(itemMove.y-puzzleItem.y) <= vaildGap){
                                        //被移动图片的左/右是否与重合的图片的右/左贴合
                                        if(Math.abs(itemMove.x-(puzzleItem.x+puzzleItem.width)) <= vaildGap){
                                            //左和右贴合
                                            itemMove.x = puzzleItem.x+puzzleItem.width
                                            itemMove.y = puzzleItem.y

                                            itemMove.topBottomLeftRightIndex.left = i
                                            puzzleItem.topBottomLeftRightIndex.right = moveNo
                                            bFindIntersected = true

                                            continue
                                        }else if(Math.abs((itemMove.x+itemMove.width)-puzzleItem.x) <= vaildGap){
                                            //右和左贴合
                                            itemMove.x = puzzleItem.x-itemMove.width
                                            itemMove.y = puzzleItem.y

                                            itemMove.topBottomLeftRightIndex.right = i
                                            puzzleItem.topBottomLeftRightIndex.left = moveNo
                                            bFindIntersected = true

                                            continue
                                        }
                                    }else if(Math.abs(itemMove.x-puzzleItem.x) <= vaildGap){
                                        //被移动图片的上/下是否与重合的图片的下/上贴合
                                        if(Math.abs(itemMove.y-(puzzleItem.y+puzzleItem.height)) <= vaildGap){
                                            //上和下贴合
                                            itemMove.y = puzzleItem.y+puzzleItem.height
                                            itemMove.x = puzzleItem.x

                                            itemMove.topBottomLeftRightIndex.top = i
                                            puzzleItem.topBottomLeftRightIndex.bottom = moveNo
                                            bFindIntersected = true

                                            continue
                                        }else if(Math.abs((itemMove.y+itemMove.height)-puzzleItem.y) <= vaildGap){
                                            //下和上贴合
                                            itemMove.y = puzzleItem.y-itemMove.height
                                            itemMove.x = puzzleItem.x

                                            itemMove.topBottomLeftRightIndex.bottom = i
                                            puzzleItem.topBottomLeftRightIndex.top = moveNo
                                            bFindIntersected = true

                                            continue
                                        }
                                    }

                                    if(!bFindIntersected){
                                        //不与任何一个有重合
                                        itemMove.x = itemMove.prevX
                                        itemMove.y = itemMove.prevY
                                    }
                                }
                            }
                        }

                        if(!bFindIntersected){
                            //不与任何一个有重合
                            itemMove.border.color = "red";

                            itemMove.topBottomLeftRightIndex.left = -1
                            itemMove.topBottomLeftRightIndex.right = -1
                            itemMove.topBottomLeftRightIndex.top = -1
                            itemMove.topBottomLeftRightIndex.bottom = -1
                        }else{
                            for(i=0; i<flicImg.contentItem.children.length; i++){
                                puzzleItem = flicImg.contentItem.children[i]

                                if(puzzleItem===rep){//不比较Repeater
                                    continue
                                }

                                if(!puzzleItem.checkIfTopBottomLeftRightOK()){
                                    break
                                }
                            }

                            if(i >= flicImg.contentItem.children.length){
                                console.info("Finish !")
                                text_finish.visible = true
                            }
                        }
                    }
                }

                Text {
                    id: text_finish
                    visible: false;
                    anchors{
                        horizontalCenter: parent.horizontalCenter;
                        verticalCenter: parent.verticalCenter}

                    color: "green"
                    text: "Finish !"
                    font.family: "微软雅黑"
                    font.pointSize: 40

                    SequentialAnimation {
                        running: text_finish.visible
                        loops: Animation.Infinite
                        NumberAnimation {
                            target: text_finish
                            property: "opacity"
                            from: 1
                            to: 0.1
                            duration: 1500
                            easing.type: Easing.OutInQuad
                        }
                        NumberAnimation {
                            target: text_finish
                            property: "opacity"
                            from: 0.1
                            to: 1
                            duration: 1500
                            easing.type: Easing.OutInQuart
                        }
                    }
                }
            }
        }
    }
}
