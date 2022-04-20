#include "qhuiversion.h"
#include "ui_qhuiversion.h"
#include "cappinfo.h"
#include <QFileInfo>

qhUiVersion::qhUiVersion(QWidget *parent) :
    QLabel(parent)
{
    SetProductVersion(true);
    SetFileName("");
}

qhUiVersion::~qhUiVersion()
{
//    delete ui;
}

QString qhUiVersion::GetInitText()
{
    return this->text();
}

void qhUiVersion::SetInitText(QString sText)
{
    this->setText(sText);
}

QString qhUiVersion::GetFileName()
{
    return mFileName;
}

void qhUiVersion::SetFileName(QString fileName)
{
    mFileName = fileName;
    QString sFile;
    if(mFileName.isEmpty()){
        sFile = QCoreApplication::applicationFilePath();
    }else{
        sFile = QCoreApplication::applicationDirPath() + "/" + mFileName;
    }

    if(QFile::exists(sFile)){
        QueryAndSetVersion(sFile);
    }else{
        this->setText(QString("文件%1缺失").arg(mFileName));
    }
}

bool qhUiVersion::GetProductVersion()
{
    return mbProductVersion;
}

void qhUiVersion::SetProductVersion(bool bEnable)
{
    mbProductVersion = bEnable;
    SetFileName(mFileName);
}

void qhUiVersion::QueryAndSetVersion(const QString &fileName)
{
    QString sFileVersion = CAPPInfo::GetFileVersion(fileName, "FileVersion");
    QString sProductVersion = CAPPInfo::GetFileVersion(fileName, "ProductVersion");

    if(mbProductVersion && !sProductVersion.isEmpty() && sFileVersion != sProductVersion){
        this->setText(sFileVersion + "/" + sProductVersion);
    }else{
        this->setText(sFileVersion);
    }
}
