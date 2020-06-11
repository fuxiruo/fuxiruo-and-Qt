#include "myscrolltext.h"
#include <QPainter>
#include <QStyleOption>
#include <QResizeEvent>
#include <QDebug>

MyScrollText::MyScrollText(QWidget *parent) : QWidget(parent)
{
    InitOffset();

    mScrollTimer.setInterval(100);
    connect(&mScrollTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
}

QSize MyScrollText::sizeHint() const
{
    return fontMetrics().size(0, text());
}

QString MyScrollText::text() const
{
    return msText;
}

void MyScrollText::SetText(QString text)
{
    msText = text;
    InitOffset();

    update();
    updateGeometry();
}

void MyScrollText::SetInterval(int v)
{
    if(v > 0){
        mScrollTimer.setInterval(v);
        mScrollTimer.start();
    }else{
        mScrollTimer.stop();
    }
}

void MyScrollText::SetScrollSteps(int v)
{
    mnScrollSteps = v;
}

void MyScrollText::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //让样式表有效
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    int textWidth = fontMetrics().width(text());
    int textHeight = fontMetrics().height();
    if(abs(mnHeadOffset) < textWidth){
        painter.drawText(mnHeadOffset, (height()-textHeight)/2, textWidth, height(), Qt::AlignLeft|Qt::AlignHCenter, text());
    }
    if(mnTailOffset > 0 && mnTailOffset < width()){
        painter.drawText(mnTailOffset, (height()-textHeight)/2, textWidth, height(), Qt::AlignLeft|Qt::AlignHCenter, text());
    }
}

void MyScrollText::resizeEvent(QResizeEvent *event)
{
    //qDebug()<<event->oldSize().width()<<width()<<mnTailOffset;

    //event->oldSize().width() == -1说明是初次显示时布局导致的大小变化
    if(event->oldSize().width() == -1 || mnTailOffset == event->oldSize().width()){
        InitOffset();
    }
}

void MyScrollText::OnTimeout()
{
    int textWidth = fontMetrics().width(text());

    //qDebug()<<mnHeadOffset<<mnTailOffset;

    if(textWidth < width()){ //文字长度小于显示区域长度
        mnHeadOffset -= mnScrollSteps;
        mnTailOffset -= mnScrollSteps;

        if(mnTailOffset < mnScrollSteps){
            mnHeadOffset = mnTailOffset;
            mnTailOffset = width();
            return;
        }
    }else{//文字长度大于显示区域长度
        mnHeadOffset -= mnScrollSteps;
        if(textWidth + mnHeadOffset < width()){
            mnTailOffset -= mnScrollSteps;

            if(mnTailOffset < mnScrollSteps){
                mnHeadOffset = mnTailOffset;
                mnTailOffset = width();
                return;
            }
        }
    }

    scroll(-1, 0);
}

void MyScrollText::InitOffset()
{
    mnHeadOffset = 0;
    mnTailOffset = width();
}
