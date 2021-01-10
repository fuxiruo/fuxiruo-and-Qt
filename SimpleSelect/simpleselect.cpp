#include "simpleselect.h"
#include "ui_simpleselect.h"
#include <QRadioButton>
#include <QLabel>
#include <QDebug>
#include <QPainter>

SimpleSelect::SimpleSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleSelect)
{
    ui->setupUi(this);

    mSelectList.clear();

    SetValueToText(QStringList()<<tr("0:停用")<<tr("1:启用"));

    connect(&mButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonToggled(int)));
}

SimpleSelect::~SimpleSelect()
{
    while(mSelectList.count() > 0){
        RemoveSelect();
    }
    delete ui;
}

void SimpleSelect::SetDirection(SimpleSelect::Direction d)
{
    ui->mLayout->setDirection((QBoxLayout::Direction)d);
}

QStringList SimpleSelect::GetValueToText()
{
    return mValueToText;
}

void SimpleSelect::SetValueToText(const QStringList &v)
{
    QStringList vaildValueToText;
    msTextList.clear();
    msValueList.clear();

    foreach(QString s, v){
        QStringList kv = s.split(":");
        if(kv.size()<2){
            kv = s.split("：");
        }

        if(kv.size()==2){
            QString sValue = kv.at(0);//数值:文字,数值合法才能保存
            QString sText = kv.at(1);
            if(msValueList.count(sValue) > 0){//值不能重复
                continue;
            }

            bool bok;
            sValue.toUInt(&bok);
            if(bok){
                vaildValueToText.append(s);
                msTextList.append(sText);
                msValueList.append(sValue);
            }
        }
    }

    mValueToText = vaildValueToText;

    AutoSetSelect();
}

void SimpleSelect::paintEvent(QPaintEvent */*event*/)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SimpleSelect::OnButtonToggled(int id)
{
    if(id != -1){
        emit SigSelectChange(msValueList.at(id).toInt(), msTextList.at(id));
    }
}

SimpleSelect::Direction SimpleSelect::GetDirection()
{
    return (SimpleSelect::Direction)ui->mLayout->direction();
}

void SimpleSelect::AddSelect()
{
    QRadioButton *radioButton = new QRadioButton(this);
    //radioButton->setObjectName(QString("radioButton_%1").arg(mSelectList.size()));
    mButtonGroup.addButton(radioButton, mSelectList.size());

    mSelectList.append(radioButton);
    ui->mLayout->addWidget(radioButton);

    if(msTextList.size() < mSelectList.size()){
        radioButton->setText(QString("label_%1").arg(mSelectList.size()-1));
        AutoAddTextAndValue(radioButton->text(), QString::number(mSelectList.size()-1));
    }else{
        radioButton->setText(msTextList.at(mSelectList.size()-1));
    }
}

void SimpleSelect::RemoveSelect()
{
    if(mSelectList.size() > 0){
        QRadioButton *radioButtonToRemove = mSelectList.back();
        mButtonGroup.removeButton(radioButtonToRemove);
        mSelectList.pop_back();
        ui->mLayout->removeWidget(radioButtonToRemove);
        delete radioButtonToRemove;
    }
}

void SimpleSelect::SetCheckNone()
{
    QRadioButton *pRadioButton;
    mButtonGroup.setExclusive(false);//要先设置才能让所有radio都不选中
    foreach (QAbstractButton* pButton, mButtonGroup.buttons()) {
        pRadioButton = (QRadioButton *)pButton;
        pRadioButton->setChecked(false);
//        qDebug()<<pRadioButton->text();
//        qDebug()<<pRadioButton->isChecked();
    }
    mButtonGroup.setExclusive(true);
}

void SimpleSelect::AutoAddTextAndValue(QString text, QString value)
{
    mValueToText<<value + ":" + text;
    msTextList.append(text);
    msValueList.append(value);
}

void SimpleSelect::AutoSetSelect()
{
    //先移除原来的
    while(mSelectList.size()){
        RemoveSelect();
    }

    while(mSelectList.size() < mValueToText.size()){
        AddSelect();
    }

    ui->mLayout->invalidate();
}
