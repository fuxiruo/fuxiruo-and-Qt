#include "formtemp.h"
#include "ui_formtemp.h"
#include <QMessageBox>
#include "modbusparameter.h"

FormTemp::FormTemp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTemp)
{
    ui->setupUi(this);

    mTimer.setInterval(8000);//超时8秒温度不可用
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
    mTimer.start();

    mSetTimer.setInterval(1000);
    mSetTimer.setSingleShot(true);
    connect(&mSetTimer, SIGNAL(timeout()), this, SLOT(OnSetTempTimeout()));
}

FormTemp::~FormTemp()
{
    delete ui;
}

void FormTemp::SetTitle(const QString &s)
{
    ui->groupBox->setTitle(s);
}

void FormTemp::SetAddr(int addr)
{
    mAddr = addr;
    ui->label_addr->setText(QString::number(addr));
}

int FormTemp::GetAddr()
{
    return mAddr;
}

void FormTemp::UpdateTemp(float_t t)
{
    mTimer.stop();
    QString sValue = QString::number(t, 'f', 1);
    sValue = QString::number(sValue.toDouble(), 'f', QLocale::FloatingPointShortest);
    ui->label_temp->setText(sValue);
    mTimer.start();
}

float_t FormTemp::GetTemp()
{
    bool bOk;
    float_t t = ui->label_temp->text().toFloat(&bOk);
    if(bOk){
        return t;
    }else{
        return -1;
    }
}

void FormTemp::UpdateSetTemp(float_t t)
{
    if(!ui->doubleSpinBoxSetTemp->hasFocus()){
        ui->doubleSpinBoxSetTemp->setValue(t);
    }

    if(mSetTimer.isActive()){
        mSetTimer.stop();
        QMessageBox::information(this, "设置成功", "设置温度成功");
    }
}

void FormTemp::ReStartTimer()
{
    mTimer.start();
}

void FormTemp::OnTimeout()
{
    ui->label_temp->setText("读取失败");
}

void FormTemp::OnSetTempTimeout()
{
    QMessageBox::warning(this, "设置超时", "等待回复超时");
}

void FormTemp::on_pushButtonSetTemp_clicked()
{
    emit SigSetTemp(GetAddr(), ui->doubleSpinBoxSetTemp->value());
    mSetTemp = ui->doubleSpinBoxSetTemp->value();
    mSetTimer.start();

    ModbusParameter mp;
    mp.SaveSetTemp(GetAddr(), mSetTemp);
}
