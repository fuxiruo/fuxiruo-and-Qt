#include "mykeyboard.h"
#include "ui_mykeyboard.h"
#include <QDebug>
#include <QLineEdit>
#include <QTableWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QPoint>

myKeyBoard::myKeyBoard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myKeyBoard)
{
    ui->setupUi(this);

    mFocusWidget = nullptr;

    InitUI();
    InitBtn();
    InitKeyMap();
    Switch(true);

    this->show();
    this->hide();

    //事件循环开始后才调用，这样就不用要求qhKeyBoard在所有控件都加载完毕后才能创建
    QTimer::singleShot(0, this, SLOT(OnStartInstallEventFilter()));
}

myKeyBoard::~myKeyBoard()
{
    delete ui;
}

void myKeyBoard::StaysOnTopHint(bool state)
{
    if (state)
    {   //软键盘保持在顶层
        setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);
        setModal(false);
    }else
    {   //取消软键盘保持在顶层，同时设置为模态，不然会被qApp->activeModalWidget()顶住输入不了
        setWindowFlags(this->windowFlags() & ~(Qt::WindowStaysOnTopHint));
        setModal(true);
    }
}

void myKeyBoard::Switch(bool v)
{
    mbSwitch = v;
}

bool myKeyBoard::eventFilter(QObject *obj, QEvent *ev)
{
    QWidget *widget = static_cast<QWidget *>(obj);
    if(widget->inherits("QTextEdit")){
        if(ev->type() == QEvent::FocusIn){
            mFocusWidget = widget;

            if(GetSwitch()){
                Show();
            }
        }
    }else if(widget->inherits("QTableWidget")){
        if(ev->type() == QEvent::FocusOut){//QTableWidget中item触发编辑后会触发QTableWidgetQEvent::FocusOut事件
            QTableWidget *tableWidget = static_cast<QTableWidget*>(widget);
            //indexWidget是通过QAbstractItemView::edit方法newc出来的，会在QTableWidget触发item双击事件之后才new
            QWidget* editor = tableWidget->indexWidget(tableWidget->currentIndex());
            if(editor && editor->hasFocus()){//确实是item得到了焦点
                mFocusWidget = editor;

                if(GetSwitch()){
                    Show();
                }
            }
        }
    }else if (ev->type() == QEvent::MouseButtonPress){
        QMouseEvent *MouseEvent = static_cast<QMouseEvent *>(ev);
        if (MouseEvent->button() == Qt::LeftButton){
            mFocusWidget = widget;

            if(GetSwitch()){
                Show();
            }
        }
    }

    return QWidget::eventFilter(obj,ev);
}

void myKeyBoard::OnStartInstallEventFilter()
{
    InstallWidgetEventFilter();

    mAppEventFilter.SetKeyBoard(this);
    qApp->installEventFilter(&mAppEventFilter);
}

void myKeyBoard::OnShow()
{
    QWidget *widget = dynamic_cast<QWidget *>(this->sender());
    if(!widget){
        return;
    }

    mFocusWidget = widget;

    if(GetSwitch()){
        Show();
    }
}

void myKeyBoard::OnWidgetDestroy(QObject *obj)
{
    mIntallWidgetList.removeOne((QWidget *)obj);

    if(mFocusWidget == obj){//收到destory时，mLastFocusWidget可能已经更新为其他了，此时不能改mLastFocusWidget
        mFocusWidget = nullptr;
    }
}

void myKeyBoard::OnKeyClick()
{
    QPushButton *button = dynamic_cast<QPushButton *>(this->sender());
    if(!button){
        return;
    }

    if(!mFocusWidget){
        return;
    }

    QStringList sKeyTexts = button->text().split("\n");
    QString sKeyTextForMap = sKeyTexts.back();
    QString sKeyText;
    if(ShiftState()){
        sKeyText = sKeyTexts.front();
    }else{
        sKeyText = sKeyTexts.back();
    }

    Qt::Key keyValue = Qt::Key_unknown;
    int keyNo = 0;
    if(-1 != mRxAz.indexIn(sKeyTextForMap)){
        if(CapsLockState() || ShiftState()){
            sKeyText = sKeyText.toUpper();
            keyNo = sKeyText.at(0).toLatin1() - 'A';
        }else{
            sKeyText = sKeyText.toLower();
            keyNo = sKeyText.at(0).toLatin1() - 'a';
        }
        keyValue = Qt::Key(Qt::Key_A + keyNo);
    }else if(-1 != mRxNumber.indexIn(sKeyTextForMap)){
        keyNo = sKeyTextForMap.at(0).toLatin1() - '0';
        keyValue = Qt::Key(Qt::Key_0 + keyNo);
        if(sKeyText == "&&"){
            sKeyText = "&";
        }
    }else{
        keyValue = mKeyMap.value(sKeyTextForMap, Qt::Key_unknown);
    }

    qDebug()<<keyValue<<" key text:'"<<sKeyText<<"'";
    if(Qt::Key_unknown != keyValue){
        if((keyValue == Qt::Key_Backspace) | (keyValue == Qt::Key_Delete)){
            sKeyText = "";
        }else if((keyValue == Qt::Key_Space)){
            sKeyText = " ";
        }else if( (keyValue == Qt::Key_Tab)){
            sKeyText = "\t";
        }else if(keyValue == Qt::Key_Enter){
            sKeyText = "\n";
        }
        SendKeyEvent(keyValue, sKeyText);
    }
}

void myKeyBoard::InitUI()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);//软键盘保持在顶层
    this->setAttribute(Qt::WA_X11DoNotAcceptFocus,true);
    this->setAttribute(Qt::WA_QuitOnClose,false); //父界面关闭，软键盘也要关闭
}

void myKeyBoard::InitBtn()
{
    QList<QPushButton *> buttons = this->findChildren<QPushButton *>();
    foreach (QPushButton *button, buttons){
        connect(button, SIGNAL(clicked(bool)), this, SLOT(OnKeyClick()));
    }

    ui->btn_capslk->setCheckable(true);
    ui->btn_shift->setCheckable(true);
}

void myKeyBoard::InitKeyMap()
{
    mRxAz.setPattern("^[a-z]|[A-Z]$");
    mRxNumber.setPattern("^[0-9]$");

    mKeyMap.insert(",", Qt::Key_Comma);
    mKeyMap.insert("<", Qt::Key_Comma);

    mKeyMap.insert(".", Qt::Key_Period);
    mKeyMap.insert(">", Qt::Key_Period);

    mKeyMap.insert("/", Qt::Key_Slash);
    mKeyMap.insert("?", Qt::Key_Slash);

    mKeyMap.insert(";", Qt::Key_Semicolon);
    mKeyMap.insert(":", Qt::Key_Semicolon);

    mKeyMap.insert("'", Qt::Key_Apostrophe);
    mKeyMap.insert("\"", Qt::Key_Apostrophe);

    mKeyMap.insert("[", Qt::Key_BracketLeft);
    mKeyMap.insert("{", Qt::Key_BracketLeft);

    mKeyMap.insert("]", Qt::Key_BracketRight);
    mKeyMap.insert("}", Qt::Key_BracketRight);

    mKeyMap.insert("\\", Qt::Key_Backslash);
    mKeyMap.insert("|", Qt::Key_Backslash);

    mKeyMap.insert("=", Qt::Key_Equal);
    mKeyMap.insert("+", Qt::Key_Equal);

    mKeyMap.insert("-", Qt::Key_Minus);
    mKeyMap.insert("_", Qt::Key_Minus);

    mKeyMap.insert("`", Qt::Key_QuoteLeft);
    mKeyMap.insert("~", Qt::Key_QuoteLeft);

    mKeyMap.insert("Tab", Qt::Key_Tab);
    mKeyMap.insert("Enter", Qt::Key_Enter);
    mKeyMap.insert("Del", Qt::Key_Delete);
    mKeyMap.insert("Backspace", Qt::Key_Backspace);
    mKeyMap.insert("", Qt::Key_Space);
}

void myKeyBoard::SetWidgetEventFilter(QWidget *widget)
{
    //键盘中本身的控件不能注册该事件的
    if(widget->window() == this->window()){
        return;
    }

    //已经注册过的控件
    if(mIntallWidgetList.contains(widget)){
        return;
    }

    if (widget->inherits("QLineEdit")){
        widget->installEventFilter(this);
    }else if (widget->inherits("QTextEdit")){
        connect(widget, SIGNAL(selectionChanged()), this, SLOT(OnShow()));
    }else if (widget->inherits("QPlainTextEdit")){
        connect(widget, SIGNAL(selectionChanged()), this, SLOT(OnShow()));
    }else if(widget->inherits("QAbstractSpinBox")){
        QLineEdit *uiInputLineEdit = widget->findChild<QLineEdit *>();
        if(uiInputLineEdit){
            uiInputLineEdit->installEventFilter(this);
        }
    }else if(widget->inherits("QTableWidget")){
        widget->installEventFilter(this);
    }

    connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(OnWidgetDestroy(QObject*)));
    mIntallWidgetList.append(widget);
}

void myKeyBoard::UpdateWidgetEvevntFilter(QWidget *newWidget)
{
    foreach (QWidget *widget, newWidget->findChildren<QWidget *>()) {
        SetWidgetEventFilter(widget);
    }
}

void myKeyBoard::InstallWidgetEventFilter()
{
    foreach (QWidget *widget, qApp->allWidgets()) {
        //键盘中本身包含这种框是不能用来注册该事件的
        SetWidgetEventFilter(widget);
    }
}

bool myKeyBoard::GetSwitch()
{
    return mbSwitch;
}

void myKeyBoard::AutoLocate()
{
    if(mFocusWidget){
        QPoint xyPoint = mFocusWidget->mapToGlobal(QPoint(0, 0));
        int titleHeight = this->frameGeometry().height() - this->height();//标题栏高度
        QRect newRect = QRect(xyPoint.x(),
                              xyPoint.y()+mFocusWidget->height()+titleHeight,
                              this->width(),
                              this->height());
        this->setGeometry(newRect);
    }
}

void myKeyBoard::Show()
{
    AutoLocate();

    this->showNormal();
}

bool myKeyBoard::ShiftState()
{
    return ui->btn_shift->isChecked();
}

bool myKeyBoard::CapsLockState()
{
    return ui->btn_capslk->isChecked();
}

void myKeyBoard::SendKeyEvent(Qt::Key keyValue, QString keyText)
{
    QKeyEvent keyPress(QKeyEvent::KeyPress, keyValue, Qt::NoModifier, keyText);
    QApplication::sendEvent(mFocusWidget, &keyPress);
}

void myKeyBoard::on_btn_close_clicked()
{
    this->close();
}
