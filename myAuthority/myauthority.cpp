#include "myauthority.h"
#include "ui_myauthority.h"
#include <QCheckBox>
#include <QLabel>
#include <QTranslator>
#include <QEvent>
#include <QSettings>
#include <QDebug>

MyAuthority::MyAuthority(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyAuthority)
{
    ui->setupUi(this);

    mbInit = false;
    SetRoles(QStringList()<<tr("Administrator")<<tr("User"));
    SetMaxAuthority(10);
}

MyAuthority::~MyAuthority()
{
    delete ui;
}

QStringList MyAuthority::GetRoles()
{
    return mRoles;
}

void MyAuthority::SetRoles(const QStringList &roles)
{
    mRoles = roles;
}

qint32 MyAuthority::GetMaxAuthority()
{
    return mMaxAuthority;
}

void MyAuthority::SetMaxAuthority(qint32 maxNum)
{
    mMaxAuthority = maxNum;
}

void MyAuthority::showEvent(QShowEvent *)
{
    if(!mbInit){
        InitUI();
        update();
    }
}

void MyAuthority::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }else{
        QWidget::changeEvent(event);
    }
}

void MyAuthority::InitUI()
{
    mbInit = true;

    //0行保存了角色
    for(int col=0; col<mRoles.size(); col++){
        QLabel *pLabel = new QLabel(this);
        pLabel->setAlignment(Qt::AlignHCenter);
        ui->gridLayout->addWidget(pLabel, 0, col+1, Qt::AlignHCenter);
    }

    //0列保存了权限
    for(int row=0; row<GetMaxAuthority(); row++){
        QLabel *pLabel = new QLabel(this);
        pLabel->setAlignment(Qt::AlignHCenter);
        ui->gridLayout->addWidget(pLabel, row+1, 0);
    }

    for(int col=0; col<mRoles.size(); col++){
        for(int row=0; row<GetMaxAuthority(); row++){
            QCheckBox *pCheckBox= new QCheckBox(this);
            //0行保存了角色，0列保存了权限，所以从1行1列开始放
            ui->gridLayout->addWidget(pCheckBox, row+1, col+1, Qt::AlignHCenter);
        }
    }

    //加载翻译文件，将Authority00翻译为设置的具体权限
//    QTranslator *translator = new QTranslator(this);
//    translator->load("myauthority_cn");
//    qApp->installTranslator(translator);
    //如果要在changeEvent中再调用retranslateUi，则translator不能为局部变量
    //也就是说，translator的生命周期还没结束时，其包含的翻译信息才有用

    retranslateUi();
}

void MyAuthority::retranslateUi()
{
    mAuthorityList.clear();
    mAuthorityList.append(tr("Authority00"));
    mAuthorityList.append(tr("Authority01"));
    mAuthorityList.append(tr("Authority02"));
    mAuthorityList.append(tr("Authority03"));
    mAuthorityList.append(tr("Authority04"));
    mAuthorityList.append(tr("Authority05"));
    mAuthorityList.append(tr("Authority06"));
    mAuthorityList.append(tr("Authority07"));
    mAuthorityList.append(tr("Authority08"));
    mAuthorityList.append(tr("Authority09"));
    mAuthorityList.append(tr("Authority10"));
    mAuthorityList.append(tr("Authority11"));
    mAuthorityList.append(tr("Authority12"));
    mAuthorityList.append(tr("Authority13"));
    mAuthorityList.append(tr("Authority14"));
    mAuthorityList.append(tr("Authority15"));
    mAuthorityList.append(tr("Authority16"));
    mAuthorityList.append(tr("Authority17"));
    mAuthorityList.append(tr("Authority18"));
    mAuthorityList.append(tr("Authority19"));
    mAuthorityList.append(tr("Authority20"));
    mAuthorityList.append(tr("Authority21"));
    mAuthorityList.append(tr("Authority22"));
    mAuthorityList.append(tr("Authority23"));
    mAuthorityList.append(tr("Authority24"));
    mAuthorityList.append(tr("Authority25"));
    mAuthorityList.append(tr("Authority26"));
    mAuthorityList.append(tr("Authority27"));
    mAuthorityList.append(tr("Authority28"));
    mAuthorityList.append(tr("Authority29"));
    mAuthorityList.append(tr("Authority30"));
    mAuthorityList.append(tr("Authority31"));
    mAuthorityList.append(tr("Authority32"));
    mAuthorityList.append(tr("Authority33"));
    mAuthorityList.append(tr("Authority34"));
    mAuthorityList.append(tr("Authority35"));
    mAuthorityList.append(tr("Authority36"));
    mAuthorityList.append(tr("Authority37"));
    mAuthorityList.append(tr("Authority38"));
    mAuthorityList.append(tr("Authority39"));
    mAuthorityList.append(tr("Authority40"));
    mAuthorityList.append(tr("Authority41"));
    mAuthorityList.append(tr("Authority42"));
    mAuthorityList.append(tr("Authority43"));
    mAuthorityList.append(tr("Authority44"));
    mAuthorityList.append(tr("Authority45"));
    mAuthorityList.append(tr("Authority46"));
    mAuthorityList.append(tr("Authority47"));
    mAuthorityList.append(tr("Authority48"));
    mAuthorityList.append(tr("Authority49"));
    mAuthorityList.append(tr("Authority50"));
    mAuthorityList.append(tr("Authority51"));
    mAuthorityList.append(tr("Authority52"));
    mAuthorityList.append(tr("Authority53"));
    mAuthorityList.append(tr("Authority54"));
    mAuthorityList.append(tr("Authority55"));
    mAuthorityList.append(tr("Authority56"));
    mAuthorityList.append(tr("Authority57"));
    mAuthorityList.append(tr("Authority58"));
    mAuthorityList.append(tr("Authority59"));
    mAuthorityList.append(tr("Authority60"));
    mAuthorityList.append(tr("Authority61"));
    mAuthorityList.append(tr("Authority62"));
    mAuthorityList.append(tr("Authority63"));
    mAuthorityList.append(tr("Authority64"));
    mAuthorityList.append(tr("Authority65"));
    mAuthorityList.append(tr("Authority66"));
    mAuthorityList.append(tr("Authority67"));
    mAuthorityList.append(tr("Authority68"));
    mAuthorityList.append(tr("Authority69"));
    mAuthorityList.append(tr("Authority70"));
    mAuthorityList.append(tr("Authority71"));
    mAuthorityList.append(tr("Authority72"));
    mAuthorityList.append(tr("Authority73"));
    mAuthorityList.append(tr("Authority74"));
    mAuthorityList.append(tr("Authority75"));
    mAuthorityList.append(tr("Authority76"));
    mAuthorityList.append(tr("Authority77"));
    mAuthorityList.append(tr("Authority78"));
    mAuthorityList.append(tr("Authority79"));
    mAuthorityList.append(tr("Authority80"));
    mAuthorityList.append(tr("Authority81"));
    mAuthorityList.append(tr("Authority82"));
    mAuthorityList.append(tr("Authority83"));
    mAuthorityList.append(tr("Authority84"));
    mAuthorityList.append(tr("Authority85"));
    mAuthorityList.append(tr("Authority86"));
    mAuthorityList.append(tr("Authority87"));
    mAuthorityList.append(tr("Authority88"));
    mAuthorityList.append(tr("Authority89"));
    mAuthorityList.append(tr("Authority90"));
    mAuthorityList.append(tr("Authority91"));
    mAuthorityList.append(tr("Authority92"));
    mAuthorityList.append(tr("Authority93"));
    mAuthorityList.append(tr("Authority94"));
    mAuthorityList.append(tr("Authority95"));
    mAuthorityList.append(tr("Authority96"));
    mAuthorityList.append(tr("Authority97"));
    mAuthorityList.append(tr("Authority98"));
    mAuthorityList.append(tr("Authority99"));

    //: 如果有多语言，可以动态修改mRoles
    for(int col=0; col<mRoles.size(); col++){
        QLabel *pLabel = dynamic_cast<QLabel *>(ui->gridLayout->itemAtPosition(0, col+1)->widget());
        if(pLabel){
            pLabel->setText(mRoles.at(col));
        }
    }

    for(int row=0; row<GetMaxAuthority(); row++){
        QLabel *pLabel = dynamic_cast<QLabel *>(ui->gridLayout->itemAtPosition(row+1, 0)->widget());
        if(pLabel){
            pLabel->setText(mAuthorityList.at(row));
            pLabel->setProperty("AuthorityIndex", row);
        }
    }

    for(int row=0; row<GetMaxAuthority(); row++){
        QLabel *pLabel = dynamic_cast<QLabel *>(ui->gridLayout->itemAtPosition(row+1, 0)->widget());
        if(pLabel){
            pLabel->setText(mAuthorityList.at(row));

            //如果权限设置为null，则隐藏该权限
            bool bVisible = false;
            if(pLabel->text().toLower()=="null"){
                bVisible = false;
            }else{
                bVisible = true;
            }
            for(int col=0; col<mRoles.size()+1; col++){
                ui->gridLayout->itemAtPosition(row+1, col)->widget()->setVisible(bVisible);
            }
        }
    }
}

bool MyAuthority::GetAuthorityByIndex(const int index, ROLE_AUTHORITY &roleAutority)
{
    if(index >= mRoles.size()){
        return false;
    }

    QLabel *pLabel = dynamic_cast<QLabel *>(ui->gridLayout->itemAtPosition(0, index+1)->widget());
    if(pLabel){
        roleAutority.Role = pLabel->text();
    }

    roleAutority.Authority.clear();
    roleAutority.AuthorityIndex.clear();
    roleAutority.AuthorityStr.clear();
    for(int row=0; row<GetMaxAuthority(); row++){
        QLabel *pLabel = dynamic_cast<QLabel *>(ui->gridLayout->itemAtPosition(row+1, 0)->widget());
        if(pLabel){
            if(pLabel->isVisible()){
                roleAutority.AuthorityStr.append(pLabel->text());
                roleAutority.AuthorityIndex.append(pLabel->property("AuthorityIndex").toInt());

                QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(row+1, index+1)->widget());
                if(pCheckBox){
                    roleAutority.Authority.append(pCheckBox->isChecked());
                }
            }
        }
    }

    return true;
}

bool MyAuthority::Load()
{
    QSettings setting("Authority.ini", QSettings::IniFormat, this);
    for(int col=0; col<mRoles.size(); col++){
        for(int row=0; row<GetMaxAuthority(); row++){
            QString sKey = QString("%1/Authority%2").arg(col).arg(QString::number(row).leftJustified(2, '0'));
            QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(row+1, col+1)->widget());
            if(pCheckBox){
                pCheckBox->setChecked(setting.value(sKey).toBool());
            }
        }
    }

    return true;
}

bool MyAuthority::Save()
{
    QSettings setting("Authority.ini", QSettings::IniFormat, this);
    for(int col=0; col<mRoles.size(); col++){
        for(int row=0; row<GetMaxAuthority(); row++){
            QString sKey = QString("%1/Authority%2").arg(col).arg(QString::number(row).leftJustified(2, '0'));
            QCheckBox *pCheckBox = dynamic_cast<QCheckBox *>(ui->gridLayout->itemAtPosition(row+1, col+1)->widget());
            if(pCheckBox){
                setting.setValue(sKey, pCheckBox->isChecked());
            }
        }
    }

    return true;
}
