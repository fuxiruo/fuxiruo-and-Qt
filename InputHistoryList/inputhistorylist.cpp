#include "inputhistorylist.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QEvent>

InputHistoryList::InputHistoryList(QWidget *parent) : QListWidget(parent)
{
    mShowUnderWidget = nullptr;

    this->setFocusPolicy(Qt::NoFocus);
    this->hide();
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));
}

void InputHistoryList::SetShowUnderWidget(QWidget *pShowUnderWidget, bool bAutoShowHide)
{
    mShowUnderWidget = pShowUnderWidget;

    if(bAutoShowHide){
        mShowUnderWidget->installEventFilter(this);//由eventFilter判断是否显示和隐藏
    }
}

void InputHistoryList::SetHistory(const QStringList &labels)
{
    this->clear();
    this->addItems(labels);
    this->setCurrentRow(0);
}

void InputHistoryList::ReShow()
{
    if(this->count()==0){
        return;
    }

    QPoint xyPoint = this->pos();

    if(mShowUnderWidget){
        xyPoint = mShowUnderWidget->mapToGlobal(QPoint(0, 0));
        //xyPoint此时是pShowUnderWidget的左上角的屏幕坐标
        xyPoint.setY(xyPoint.y() + mShowUnderWidget->height());
        //我们希望弹出在pShowUnderWidget左下方，所以要加上pShowUnderWidget->height()
        if(this->parentWidget()){
            xyPoint = this->parentWidget()->mapFromGlobal(xyPoint);
        }
        int height = this->visualItemRect(this->item(0)).height()*this->count();
        //setGeometry中的x、y是相对于parenWidget而言的，所以需要将屏幕坐标转换成parenWidget上对应的坐标
        this->setGeometry(xyPoint.x(), xyPoint.y(), mShowUnderWidget->width(), height);
    }

    this->setCurrentRow(0);
    this->show();
    this->raise();
}

void InputHistoryList::OnItemClicked(QListWidgetItem *item)
{
    this->hide();

    QSpinBox *spinBox = dynamic_cast<QSpinBox *>(mShowUnderWidget);
    if(spinBox){
        spinBox->setValue(item->text().toInt());
        return;
    }

    QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(mShowUnderWidget);
    if(lineEdit){
        lineEdit->setText(item->text());
        return;
    }
}

bool InputHistoryList::eventFilter(QObject *obj, QEvent *ev)
{
    QWidget *widget = static_cast<QWidget *>(obj);
    if(ev->type() == QEvent::FocusIn){
        ReShow();
    }else if(ev->type() == QEvent::HoverEnter){
        if(widget->hasFocus()){
            ReShow();
        }
    }else if(ev->type() == QEvent::FocusOut){
        this->hide();
    }else if(ev->type() == QEvent::Move){
        if(this->isVisible()){
            ReShow();
        }
    }
    return QWidget::eventFilter(obj,ev);
}
