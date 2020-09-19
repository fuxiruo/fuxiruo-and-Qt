#include "userdefined.h"
#include "ui_userdefined.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QDebug>
#include <QInputDialog>

#include "commonfunc.h"

UserDefined::UserDefined(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDefined)
{
    ui->setupUi(this);

    Init();
}

UserDefined::~UserDefined()
{
    Save();

    delete ui;
}

void UserDefined::showEvent(QShowEvent *event)
{
    Load();

    QWidget::showEvent(event);
}

void UserDefined::OnBtnClick()
{
    QPushButton *pButton = static_cast<QPushButton *>(this->sender());
    int nRow = pButton->objectName().remove("pushBtn").toInt();

    QLineEdit *pLineEdit = static_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(nRow, E_COL_OF_LINEEDIT)->widget());

    if(!pLineEdit->text().isEmpty()){
        emit SigSend(pLineEdit->text());
    }
}

void UserDefined::OnTextEdited(const QString &)
{
    QLineEdit *pLineEdit = static_cast<QLineEdit *>(this->sender());
    int nRow = pLineEdit->objectName().remove("lineEdit").toInt();
    mChangeColList.insert(nRow);
}

void UserDefined::OnLabelLinkActivated(const QString &link)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("请输入注释："),
                                      "", QLineEdit::Normal,
                                      "", &ok);
    if (ok && !text.isEmpty()){
        int nRow = link.toInt();
        mChangeColList.insert(nRow);
        QPushButton *pPushBtn = static_cast<QPushButton *>(ui->gridLayout->itemAtPosition(nRow, E_COL_OF_SEND_BTN)->widget());
        pPushBtn->setText(text);
    }
}

void UserDefined::Add()
{
    QLabel *pLabel = new QLabel(this);
    pLabel->setText(QString("<a href=%1>%1</a>").arg(mRowCount));
    ui->gridLayout->addWidget(pLabel, mRowCount, E_COL_OF_INDEX);

    QLineEdit *pLineEdit = new QLineEdit(this);
    pLineEdit->setObjectName(QString("lineEdit%1").arg(mRowCount));
    ui->gridLayout->addWidget(pLineEdit, mRowCount, E_COL_OF_LINEEDIT);

    QPushButton *pPushBtn = new QPushButton(this);
    pPushBtn->setObjectName(QString("pushBtn%1").arg(mRowCount));
    pPushBtn->setText("Send");
    ui->gridLayout->addWidget(pPushBtn, mRowCount, E_COL_OF_SEND_BTN);

    connect(pLabel, SIGNAL(linkActivated(QString)), this, SLOT(OnLabelLinkActivated(QString)));
    connect(pLineEdit, SIGNAL(textEdited(QString)), this, SLOT(OnTextEdited(QString)));
    connect(pPushBtn, SIGNAL(clicked()), this, SLOT(OnBtnClick()));

    mRowCount++;
}

void UserDefined::Init()
{
    mRowCount = 0;

    for(int i=0; i<100; i++){
        Add();
    }
}

void UserDefined::Save()
{
    QSettings setting(CommonFunc::GetAppPath()+"/userdefined.ini", QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    QSet<int>::const_iterator i = mChangeColList.constBegin();
    while (i != mChangeColList.constEnd()) {
        int row = *i;
        QLineEdit *pLineEdit = static_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(row, E_COL_OF_LINEEDIT)->widget());
        setting.setValue(QString("%1/lineEdit%2").arg(this->objectName()).arg(row),
                         pLineEdit->text());

        QPushButton *pPushBtn = static_cast<QPushButton *>(ui->gridLayout->itemAtPosition(row, E_COL_OF_SEND_BTN)->widget());
        setting.setValue(QString("%1/pushBtn%2").arg(this->objectName()).arg(row),
                         pPushBtn->text());

        ++i;
    }
}

void UserDefined::Load()
{
    QSettings setting(CommonFunc::GetAppPath()+"/userdefined.ini", QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    for(int i=0; i<mRowCount; i++){
        QString text = setting.value(QString("%1/lineEdit%2").arg(this->objectName()).arg(i)).toString();
        if(!text.isEmpty()){
            QLineEdit *pLineEdit = static_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(i, E_COL_OF_LINEEDIT)->widget());
            pLineEdit->setText(text);
        }

        text = setting.value(QString("%1/pushBtn%2").arg(this->objectName()).arg(i)).toString();
        if(!text.isEmpty()){
            QPushButton *pPushBtn  = static_cast<QPushButton *>(ui->gridLayout->itemAtPosition(i, E_COL_OF_SEND_BTN)->widget());
            pPushBtn->setText(text);
        }
    }
}
