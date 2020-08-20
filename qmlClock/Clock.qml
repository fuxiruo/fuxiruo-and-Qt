import QtQuick 2.9

//Defining Custom QML Types for Re-use,文件名要大写开头
Item {
    width: 400
    height: width

    Rectangle{
        width: parent.width
        height: parent.height
        radius: width/2
        border.width: 1
        border.color: "black"

        property int hour_scale: 20//时针刻度长度
        property int minute_scale: hour_scale/2//分针刻度长度
        property int second_hand: height/2-2*hour_scale//秒针长度
        property int minute_hand: second_hand-10//分针长度
        property int hour_hand: minute_hand*3/4//时针长度

        //中心圆圈
        Rectangle{
            width: 10
            height: width
            radius: width/2
            color: "black"
            anchors.centerIn: parent
        }

        //生成时钟刻度
        Canvas	{
            id: clock_painter
            width:	parent.width
            height:	width

            anchors.centerIn: parent.Center

            onPaint:	{
                var	ctx	= getContext("2d")
                var bigR = height/2;
                var smallR = bigR - parent.hour_scale

                //	setup	the	stroke
                ctx.strokeStyle	= "black"
                ctx.lineWidth = 2

                //重置，否则上一次绘制的还在
                ctx.reset()

                //	create	a	path
                ctx.beginPath()

                ctx.translate(width/2, height/2)//中心坐标转为(0,0)

                //画时针刻度
                var i,angle=0;

                for(i=0; i<12; i++){
                    angle = i * Math.PI/6//30度为一格

                    //Math.sin(Math.PI/6),角度30的正弦
                    ctx.moveTo(bigR*Math.sin(angle), -bigR*Math.cos(angle))
                    ctx.lineTo(smallR*Math.sin(angle), -smallR*Math.cos(angle))
                    //	stroke	path
                    ctx.stroke()
                }

                //画时钟刻度数字
                var strWidth,strHeight,text
                ctx.save()//要旋转坐标，先保存
                for(i=1; i<=12; i++){
                    text = String(i)
                    strWidth = ctx.measureText(text).width
                    ctx.rotate(Math.PI/6);
                    ctx.strokeText(text , -strWidth/2, -bigR+parent.hour_scale+10);
                }
                ctx.restore()//恢复旋转的坐标

                //画分针刻度
                smallR = bigR - parent.minute_scale
                for(i=0; i<60; i++){
                    angle = i * Math.PI/30//6度为一格

                    //Math.sin(Math.PI/6),角度30的正弦
                    ctx.moveTo(bigR*Math.sin(angle), -bigR*Math.cos(angle))
                    ctx.lineTo(smallR*Math.sin(angle), -smallR*Math.cos(angle))
                    //	stroke	path
                    ctx.stroke()
                }

                //画时针, getHours()是24小时的
                var nowDate = new Date()
                angle = nowDate.getHours()%12 * Math.PI/6
                ctx.moveTo(0, 0)
                ctx.lineTo(parent.hour_hand*Math.sin(angle), -parent.hour_hand*Math.cos(angle))
                //	stroke	path
                ctx.stroke()

                //画分针
                angle =  2*Math.PI * nowDate.getMinutes()/60
                ctx.moveTo(0, 0)
                ctx.lineTo(parent.minute_hand*Math.sin(angle), -parent.minute_hand*Math.cos(angle))
                //	stroke	path
                ctx.stroke()

                //画秒针
                angle =  2*Math.PI * nowDate.getSeconds()/60
                ctx.moveTo(0, 0)
                ctx.lineTo(parent.minute_hand*Math.sin(angle), -parent.minute_hand*Math.cos(angle))
                //	stroke	path
                ctx.stroke()

                ctx.closePath()
            }
        }
    }

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: {
            clock_painter.requestPaint()
        }
    }
}
