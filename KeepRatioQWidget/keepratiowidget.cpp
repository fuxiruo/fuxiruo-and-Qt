#include "keepratiowidget.h"
#include <QResizeEvent>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

KeepRatioWidget::KeepRatioWidget(QWidget *parent) : QWidget(parent)
{
    mbInitChildWidgetRatio = false;
    mbHasAdjustRezise = false;
}

/**
 * @brief KeepRatioWidget::paintEvent
 * 继承自QWidget的组件样式表无用的解决方法
 */
void KeepRatioWidget::paintEvent(QPaintEvent */*event*/)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void KeepRatioWidget::InitChildWidgetRatio()
{
//    qDebug()<<mbInitChildWidgetRatio<<this->minimumHeight()<<this->minimumWidth();
    if(!mbInitChildWidgetRatio){
        if(this->minimumWidth() != 0 && this->minimumHeight() != 0){
            QList<QWidget *> widgetList =  this->findChildren<QWidget *>();
            foreach (QWidget *wdiget, widgetList) {
                SetChildWidgetRatio(wdiget);
            }
        }
        mbInitChildWidgetRatio = true;
    }
}

void KeepRatioWidget::SetChildWidgetRatio(QWidget *widget)
{
    if(widget->parentWidget()){
        if(widget->parentWidget() != this){
            SetChildWidgetRatio(widget->parentWidget());
        }else{
            if(!mChildWidgetRatio.contains(widget->objectName())){
                CHILD_WIDGET_RATIO childWidgetRatio;
                childWidgetRatio.xRatio = widget->x()*1.0/this->minimumWidth();
                childWidgetRatio.yRatio = widget->y()*1.0/this->minimumHeight();
                childWidgetRatio.wRatio = widget->width()*1.0/this->minimumWidth();
                childWidgetRatio.hRatio = widget->height()*1.0/this->minimumHeight();
                mChildWidgetRatio.insert(widget->objectName(), childWidgetRatio);
//                qDebug()<<widget->objectName()<<"init pos:"<<widget->geometry();
            }
        }
    }
}

void KeepRatioWidget::MoveParentWidget(QWidget *widget, QList<QWidget *> &widgetHasMove)
{
    if(widget->parentWidget()){
        if(widget->parentWidget() != this){
            MoveParentWidget(widget->parentWidget(), widgetHasMove);
        }else{
            if(!widgetHasMove.contains(widget)){
                if(mChildWidgetRatio.contains(widget->objectName())){
                    CHILD_WIDGET_RATIO childWidgetRatio = mChildWidgetRatio[widget->objectName()];
                    widget->adjustSize();//如果从隐藏到可见，大小又调整小了，需要重新adjustSize一下

                    //没有跟着缩放，需要额外调整x,y的位置
                    int adjustX = (childWidgetRatio.wRatio*this->width()-widget->width())/2;
                    int adjustY = (childWidgetRatio.wRatio*this->height()-widget->height())/2;

                    widget->move(childWidgetRatio.xRatio*this->width()+adjustX, childWidgetRatio.yRatio*this->height()+adjustY);
//                    qDebug()<<"move:"<<widget->objectName()<<widget->pos();
                }
                widgetHasMove.append(widget);
            }
        }
    }
}

void KeepRatioWidget::WidgetSetGeometry(QWidget *widget, QList<QWidget *> &widgetHasMove)
{
    if(widget->parentWidget()){
        if(widget->parentWidget() != this){
            WidgetSetGeometry(widget->parentWidget(), widgetHasMove);
        }else{
            if(!widgetHasMove.contains(widget)){
                if(mChildWidgetRatio.contains(widget->objectName())){
                    CHILD_WIDGET_RATIO childWidgetRatio = mChildWidgetRatio[widget->objectName()];
                    widget->setGeometry(childWidgetRatio.xRatio*this->width(), childWidgetRatio.yRatio*this->height(),
                                        childWidgetRatio.wRatio*this->width(), childWidgetRatio.hRatio*this->height());
//                    qDebug()<<"set:"<<widget->objectName()<<widget->geometry();
                }
                widgetHasMove.append(widget);
            }
        }
    }
}

void KeepRatioWidget::resizeEvent(QResizeEvent *event)
{
    InitChildWidgetRatio();

//    qDebug()<<"old size:"<<event->oldSize()<<",new size:"<<event->size()<<this->geometry();
    if(this->minimumWidth() != 0 && this->minimumHeight() != 0){
        if(!mbHasAdjustRezise){
            //setGeometry后会调用resizeEvent，mbHasAdjustRezise防止无限循环
            mbHasAdjustRezise = true;

            QSize old_size = event->size();
            QSize new_size = event->size();

            int adjustX,adjustY;
            if(new_size.width()<this->minimumWidth()*new_size.height()/this->minimumHeight()){
                //宽度不足，则以宽度来计算
                new_size.setHeight(this->minimumHeight() * new_size.width() / this->minimumWidth());

                //竖直方向居中
                adjustX = this->geometry().x();
                adjustY = this->geometry().y() + (old_size.height()-new_size.height())/2;
           }else {
                //否则以高度来计算
                new_size.setWidth(this->minimumWidth() * new_size.height() / this->minimumHeight());
                //水平方向居中
                adjustX = this->geometry().x() + (old_size.width()-new_size.width())/2;
                adjustY = this->geometry().y();
            }
            this->setGeometry(adjustX, adjustY, new_size.width(), new_size.height());

//                qDebug()<<"adjust:"<<this->geometry();
            mbHasAdjustRezise = false;
        }
    }

    /*
    如果该QWidget是没有进行布局的，还有其他QWidget，
    还可以调用其他QWidget的move或者setGeometry来让它们也按比例移动或者缩放
    还可以指定更加明确的QWidget类型，如你的自定控件，
    */
//    qDebug("deal children");
    QList<QWidget *> widgetList =  this->findChildren<QWidget *>();
    QList<QWidget *> widgetHasMove;
    foreach (QWidget *wdiget, widgetList) {
        //只对父控件是本控件的进行操作
        //MoveParentWidget(wdiget, widgetHasMove);//只移动不缩放子控件
        WidgetSetGeometry(wdiget, widgetHasMove);//移动且缩放子控件
    }
    QWidget::resizeEvent(event);
}
