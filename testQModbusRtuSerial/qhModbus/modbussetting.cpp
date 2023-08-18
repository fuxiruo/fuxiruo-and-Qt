#include "modbussetting.h"
#include "ui_modbussetting.h"
#include "modbusparameter.h"

ModbusSetting::ModbusSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusSetting)
{
    ui->setupUi(this);

    ModbusParameter mp;
    StModbusSerialParameter smp;
    mp.LoadSerialParameter(smp);

    ui->parityCombo->setCurrentIndex(smp.parity);
    ui->baudCombo->setCurrentText(QString::number(smp.baud));
    ui->dataBitsCombo->setCurrentText(QString::number(smp.dataBits));
    ui->stopBitsCombo->setCurrentText(QString::number(smp.stopBits));
    ui->timeoutSpinner->setValue(smp.responseTime);
    ui->retriesSpinner->setValue(smp.numberOfRetries);
}

ModbusSetting::~ModbusSetting()
{
    delete ui;
}
