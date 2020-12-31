#include "messageimage.h"
#include "ui_qhmessageimage.h"
#include <QDir>
#include <QDebug>

MessageImage::MessageImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qhMessageImage)
{
    ui->setupUi(this);

    SetLoopShowInterval(2000);
    connect(&mLoopShowTimer, SIGNAL(timeout()), this, SLOT(OnLoopShowTimeout()));

    SetQueryInterval(0);
    SetModuleCounter(32);
    connect(&mQueryTimer, SIGNAL(timeout()), this, SLOT(OnQueryTimeout()));

    SetDir("");

    connect(&mButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonGroupClicked(int)));
}

MessageImage::~MessageImage()
{
    delete ui;
}

void MessageImage::SetDir(QString sDir)
{
    msDir = sDir;
}

QString MessageImage::Dir()
{
    return msDir;
}

void MessageImage::SetLoopShowInterval(int n)
{
    mLoopShowTimer.setInterval(n);
    if(n > 0){
        mLoopShowTimer.start();
    }else{
        mLoopShowTimer.stop();
    }
}

int MessageImage::GetLoopShowInterval()
{
    return mLoopShowTimer.interval();
}

void MessageImage::SetQueryInterval(int n)
{
    mQueryTimer.setInterval(n);
    if(n > 0){
        mQueryTimer.start();
    }else{
        mQueryTimer.stop();
    }
}

int MessageImage::GetQueryInterval()
{
    return mQueryTimer.interval();
}

quint32 MessageImage::GetModuleCounter()
{
    return mModuleCount;
}

void MessageImage::SetModuleCounter(quint32 v)
{
    mModuleCount = v;
}

void MessageImage::ModuleData(quint8 midx, quint8 data)
{
    if(data == 0){
        RemoveErr(midx);
    }else{
        AddErr(midx, data);
    }
}

void MessageImage::leaveEvent(QEvent *event)
{
    if(!mLoopShowTimer.isActive()){
        if(mLoopShowTimer.interval() > 0){
            mLoopShowTimer.start();
        }
    }
    QWidget::leaveEvent(event);
}

void MessageImage::OnLoopShowTimeout()
{
    if(ui->stackedWidget_img->count() == 0){
        return;
    }

    int newIndex = ui->stackedWidget_img->currentIndex();
    newIndex++;
    if(newIndex >= ui->stackedWidget_img->count()){
        newIndex = 0;
    }
    ui->stackedWidget_img->setCurrentIndex(newIndex);

    ERRWidget *errWidget = dynamic_cast<ERRWidget *>(ui->stackedWidget_img->currentWidget());
    foreach (QAbstractButton *button, mButtonGroup.buttons()) {
        if(mButtonGroup.id(button) == errWidget->whatsThis().toInt()){
            button->setChecked(true);
        }else{
            button->setChecked(false);
        }
    }
}

void MessageImage::OnQueryTimeout()
{

}

void MessageImage::OnButtonGroupClicked(int id)
{
    if(msShowErrWidgets.contains(id)){
        if(msShowErrWidgets[id] != ui->stackedWidget_img->currentWidget()){
            ui->stackedWidget_img->setCurrentWidget(msShowErrWidgets[id]);
        }

        if(mLoopShowTimer.isActive()){
            mLoopShowTimer.stop();
        }
    }
}

void MessageImage::FromReadModule(QByteArray array)
{
    if(array.size() < 3){
        return;
    }

    quint8 addr = array.at(1);
    quint8 value = array.at(2);

    if(addr >= mModuleCount)
        return;

    if(value > 0){
        AddErr(addr, value);
    }else{
        RemoveErr(addr);
    }
}

void MessageImage::AddErr(quint8 moduleIdx, RecErrorDefine err)
{
    ERRWidget *errWidget;
    QString sPicPath = GetPicDir() + "/" + err.PicturePath;

    quint8 tempModuleIdx;

    //已经显示的错误就只是更新图片
    if(msShowErrWidgets.contains(moduleIdx)){
        if(IsErrCodeInList(err.Idx, tempModuleIdx)){
            //有一个tempModuleIdx已经报错了err.Idx，现在moduleIdx同样报错err.idx，可以移除moduleIdx的报错
            if(moduleIdx != tempModuleIdx){
                RemoveErr(moduleIdx);
                return;
            }
        }

        UpdateModuleErr(moduleIdx, err);
        return;
    }

    //已存在的报错不重复添加显示
    if(IsErrCodeInList(err.Idx, tempModuleIdx)){
        return;
    }

    if(msIdleErrWidgets.count() > 0){
        errWidget = msIdleErrWidgets.dequeue();
//        qDebug()<<"errWidget = msIdleErrWidgets.dequeue()";
    }else{
        errWidget = new ERRWidget(this);
    }
    errWidget->setWhatsThis(QString::number(moduleIdx));
    errWidget->SetErrCode(err.Idx);
    errWidget->SetBorderPic(sPicPath);
    errWidget->SetErrInfo(err.Message);

    msShowErrWidgets.insert(moduleIdx, errWidget);
    ui->stackedWidget_img->addWidget(errWidget);
    AddButton(moduleIdx, err);

    if(this->isHidden()){
        this->show();
    }

    //从0变成1，通知外部有报错
    if(msShowErrWidgets.count() == 1){
        NotifyModuleErrStatus(true);
    }
}

void MessageImage::AddButton(quint8 moduleIdx, RecErrorDefine err)
{
    QPushButton *button;
    if(msIdleButton.count() > 0){
        button = msIdleButton.dequeue();
//        qDebug()<<"button = msIdleButton.dequeue()";
    }else{
        button = new QPushButton(ui->frame_btn);
    }
    button->setText(QString::number(err.Idx));
    button->adjustSize();
    button->setCheckable(true);

    ui->horizontalLayout_btn->addWidget(button);
    button->show();
    mButtonGroup.addButton(button, moduleIdx);
}

void MessageImage::RemoveErr(quint8 moduleIdx)
{
    if(msShowErrWidgets.contains(moduleIdx)){
        ERRWidget *errWidget = msShowErrWidgets[moduleIdx];
        ui->stackedWidget_img->removeWidget(errWidget);
        msIdleErrWidgets.enqueue(errWidget);
        msShowErrWidgets.remove(moduleIdx);

//        qDebug()<<"remove widgets:"<<errWidget->GetErrCode();
        RemoveButton(moduleIdx);

        //从1变成0，通知外部没有报错了
        if(msShowErrWidgets.count() == 0){
            if(this->isVisible()){
                this->hide();
            }
            NotifyModuleErrStatus(false);
        }
    }
}

void MessageImage::RemoveButton(quint8 moduleIdx)
{
    QPushButton *button;
    if(mButtonGroup.button(moduleIdx)){
        button = dynamic_cast<QPushButton *>(mButtonGroup.button(moduleIdx));
        ui->horizontalLayout_btn->removeWidget(button);
        button->hide();
        mButtonGroup.removeButton(mButtonGroup.button(moduleIdx));
        msIdleButton.enqueue(button);
//        qDebug()<<"remove button:"<<button->text();
    }
}

//检查某个报错是否已经存在
bool MessageImage::IsErrCodeInList(quint32 code, quint8 &moduleIdx)
{
    QMap<quint8, ERRWidget *>::const_iterator i = msShowErrWidgets.constBegin();
    while (i != msShowErrWidgets.constEnd()) {
        if(i.value()->GetErrCode()==code){
            moduleIdx = i.key();
            return true;
        }
        ++i;
    }
    return false;
}

void MessageImage::NotifyModuleErrStatus(bool bHasErr)
{
    emit SigModuleErrStatus(bHasErr);
}

QString MessageImage::GetPicDir()
{
    static QString sAppDir = QApplication::applicationDirPath();
    QString sPicDir = sAppDir + "/" + msDir;
    QDir picDir(sPicDir);
    if(picDir.exists()){
        return sPicDir;
    }else{
        return sAppDir;
    }
}

void MessageImage::UpdateModuleErr(quint8 moduleIdx, RecErrorDefine err)
{
    ERRWidget *errWidget = msShowErrWidgets[moduleIdx];
    QString sPicPath = GetPicDir() + "/" + err.PicturePath;

//    qDebug()<<moduleIdx<<":"<<err.Message<<","<<errWidget->GetErrInfo();
    quint32 preErrCode = errWidget->GetErrCode();
    errWidget->SetErrCode(err.Idx);
    if(sPicPath != errWidget->GetBorderPic()){
        errWidget->SetBorderPic(sPicPath);
    }
    if(preErrCode != err.Idx || err.Message != errWidget->GetErrInfo()){
        errWidget->SetErrInfo(err.Message);
    }
    errWidget->adjustSize();

    if(mButtonGroup.button(moduleIdx)){
        QPushButton *button = dynamic_cast<QPushButton *>(mButtonGroup.button(moduleIdx));
        if(button->text() != QString::number(err.Idx)){
            button->setText(QString::number(err.Idx));
        }
    }
}

void MessageImage::AddErr(quint8 moduleIdx, quint8 err)
{
    RecErrorDefine d;
    d.Idx = err;
    d.Message = QString("Undefine message %1").arg(err);
    d.PicturePath = QString("%1.png").arg(err);
    AddErr(moduleIdx, d);
}
