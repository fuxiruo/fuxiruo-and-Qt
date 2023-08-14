#include "qhimage.h"
#include "ui_qhimage.h"
#include <QDebug>

#define STYLE_STR_image             ".QFrame#qhImagePicFrame{image: url(%1);image-position: center;}"
#define STYLE_STR_background_image  ".QFrame#qhImagePicFrame{background-image: url(%1);}"
#define STYLE_STR_border_image      ".QFrame#qhImagePicFrame{border-image: url(%1);}"

qhImage::qhImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qhImage)
{
    ui->setupUi(this);

    mShowType = Preferred;
}

qhImage::~qhImage()
{
    delete ui;
}

QString qhImage::PicPath() const
{
    return mPicPath;
}

void qhImage::SetPicPath(const QString &pic)
{
    mPicPath = pic;
    UpdatePic();
}

qhImage::E_ShowType qhImage::ShowType() const
{
    return mShowType;
}

void qhImage::SetShowType(qhImage::E_ShowType st)
{
    mShowType = st;
    UpdatePic();
}

void qhImage::UpdatePic()
{
    QString sty;
    if(mShowType == Preferred){
        sty = QString(STYLE_STR_image).arg(mPicPath);
    }else if(mShowType == Expanding){
        sty = QString(STYLE_STR_border_image).arg(mPicPath);
    }else{
        sty = QString(STYLE_STR_background_image).arg(mPicPath);
    }

    if(sty != ui->qhImagePicFrame->styleSheet()){
        ui->qhImagePicFrame->setStyleSheet(sty);
        //qDebug()<<sty;
    }
}
