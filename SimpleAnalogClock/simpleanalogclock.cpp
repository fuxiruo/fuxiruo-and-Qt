#include "simpleanalogclock.h"
#include <QPainter>

SimpleAnalogClock::SimpleAnalogClock(QWidget *parent) : QWidget(parent)
{
    mLastDateTime = QDateTime::currentDateTime();

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
    mTimer.start(500);
}

void SimpleAnalogClock::paintEvent(QPaintEvent */*event*/)
{
    int side = qMin(this->width(), this->height());

    QPainter painter(this);
    //抗锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //将坐标(this->width() / 2, this->height() / 2)转换为(0,0)
    painter.translate(this->width() / 2, this->height() / 2);
    //将大小转换为200为单位，此时坐标范围为(-100,-100)~(100,100)
    int radio = 200;
    painter.scale(side / 200.0, side / 200.0);
    int r = radio/2 - 1;

    //画椭圆，其实是一个圆
    painter.drawEllipse(-r, -r, 2*r, 2*r);

    int hourMark = 10;
    int textSize = 10;
    QRect rectangle = QRect(-textSize/2, -r+hourMark+1, textSize+1, textSize+1);
    QRect rectangleText;

    painter.save();
    for(int i=1; i<=12; i++){
        //时针刻度
        painter.rotate(360.0/12);
        painter.drawLine(0, -r, 0, -r+hourMark);

        //rectangle是期望文字显示在的矩形区域，rectangleText是实际需要的矩形区域
        rectangleText = painter.boundingRect(rectangle, Qt::AlignCenter, QString::number(i));
        painter.drawText(rectangleText, Qt::AlignCenter, QString::number(i));
    }

    int minMark = 5;
    for(int i=0; i<60; i++){
        //分针刻度
        if(i%5){
            painter.drawLine(0, -r, 0, -r+minMark);
        }
        painter.rotate(360.0/60);
    }
    painter.restore();

    mLastDateTime = QDateTime::currentDateTime();
    int minHand = r-hourMark-rectangleText.height();
    int hourHand = minHand*2/3;
    int secHand = r-hourMark;
    //显示时针
    painter.save();
    float hourAndMin = (mLastDateTime.time().hour()) % 12;//多少小时,对12取余处理24小时的格式
    hourAndMin += mLastDateTime.time().minute()/60.0;
    hourAndMin *= 60.0/12;//1小时时针转的角度相当于分针5分钟转的角度
    painter.rotate(360.0/60 * hourAndMin);
    painter.drawLine(0, 0, 0, -hourHand);
    painter.restore();

    //显示分针
    painter.save();
    painter.rotate(360.0/60 * mLastDateTime.time().minute());
    painter.drawLine(0, 0, 0, -minHand);
    painter.restore();

    //显示秒针
    painter.save();
    painter.rotate(360.0/60 * mLastDateTime.time().second());
    painter.drawLine(0, 0, 0, -secHand);
    painter.restore();

    painter.setBrush(Qt::SolidPattern);
    painter.drawEllipse(QPoint(0,0), minMark, minMark);
}

void SimpleAnalogClock::OnTimeout()
{
    QDateTime qNowDateTime = QDateTime::currentDateTime();
//    qDebug()<<qNowDateTime;
//    qDebug()<<mLastDateTime;
    if(qNowDateTime.msecsTo(mLastDateTime) < -900){
//        qDebug("update");
        this->update();
    }
}
