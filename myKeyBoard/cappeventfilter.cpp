#include "cappeventfilter.h"
#include <QEvent>
#include <QLineEdit>
#include <QDebug>
#include <qapplication.h>
#include "mykeyboard.h"

CAppEventFilter::CAppEventFilter(QObject *parent) : QObject(parent)
{
    mpKeyBoard = nullptr;
}

void CAppEventFilter::SetKeyBoard(myKeyBoard *pKeyBoard)
{
    mpKeyBoard = pKeyBoard;
}

bool CAppEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        //qDebug()<<event->type();
        //    软件盘设置了Qt::WindowStaysOnTopHint，模态对话框弹出时会被软键盘遮挡，
        //    导致模态对话框不能关闭，所以要特殊处理模态对话框弹出的情况
        if(mpKeyBoard){
            if (qApp->activeModalWidget() != NULL && qApp->activeModalWidget()->isVisible())
            {
                mpKeyBoard->StaysOnTopHint(false);
                mpKeyBoard->UpdateWidgetEvevntFilter(qApp->activeModalWidget());
            }else
            {
                mpKeyBoard->StaysOnTopHint(true);
            }
        }
    }else if(event->type() == QEvent::Show){
        //qDebug()<<"widget show:"<<watched->objectName();
        QWidget *widget = static_cast<QWidget *>(watched);
        if(widget){
            //qDebug()<<"widget window:"<<widget->window();
            mpKeyBoard->SetWidgetEventFilter(widget);
        }
    }

    return QObject::eventFilter(watched, event);
}
