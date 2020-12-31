#include "errwidget.h"
#include "ui_errwidget.h"
#include <QResizeEvent>
#include <QDebug>

ERRWidget::ERRWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ERRWidget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);

    mRatioMode = Qt::KeepAspectRatio;//图片缩放是否保持比例
}

ERRWidget::~ERRWidget()
{
    delete ui;
}

void ERRWidget::SetBorderPic(QString sBorderPic)
{
    msBorderPic = sBorderPic;
    SetBackgroundRolePic();
    this->adjustSize();
}

void ERRWidget::SetErrCode(quint32 code)
{
    mErrCode = code;
}

quint32 ERRWidget::GetErrCode()
{
    return mErrCode;
}

QString ERRWidget::GetBorderPic()
{
    return msBorderPic;
}

void ERRWidget::SetErrInfo(QString sErrInfo)
{
    msErrInfo = sErrInfo;
    ui->label_errInfo->setText(QString("【%1】 %2").arg(mErrCode).arg(msErrInfo));
}

QString ERRWidget::GetErrInfo()
{
    return msErrInfo;
}

void ERRWidget::resizeEvent(QResizeEvent *event)
{
    SetBackgroundRolePic();

    QWidget::resizeEvent(event);
}

void ERRWidget::SetBackgroundRolePic()
{
    QPixmap pixmap1(msBorderPic);

    if(mRatioMode == Qt::KeepAspectRatio){
        if(!pixmap1.isNull()){
            QPixmap pixmap = pixmap1.scaled(this->rect().width(), this->rect().height()-ui->label_errInfo->rect().height(),
                                            Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPalette palette;
            palette.setBrush(ui->label_errimg->backgroundRole(),QBrush(pixmap));
            ui->label_errimg->setPalette(palette);
            ui->label_errimg->setAutoFillBackground(true);
            ui->label_errimg->setGeometry(this->rect().width()/2- pixmap.width()/2,
                                                  (this->rect().height() - ui->label_errInfo->rect().height())/2 - pixmap.height()/2,
                                                  pixmap.width(), pixmap.height());
        }else{
            QPalette palette;
            palette.setBrush(ui->label_errimg->backgroundRole(),Qt::NoBrush);
            ui->label_errimg->setPalette(palette);
        }
    }else{
        if(!pixmap1.isNull()){
           QPixmap pixmap = pixmap1.scaled(this->width(), this->height()-ui->label_errInfo->rect().height(),
                                           Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           QPalette palette;
           palette.setBrush(this->backgroundRole(),QBrush(pixmap));
           this->setPalette(palette);
       }else{
           QPalette palette;
           palette.setBrush(this->backgroundRole(), Qt::NoBrush);
           this->setPalette(palette);
       }
    }
}

