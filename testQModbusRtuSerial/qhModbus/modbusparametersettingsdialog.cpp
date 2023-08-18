#include "modbusparametersettingsdialog.h"
#include "ui_modbusparametersettingsdialog.h"
#include "modbusparameter.h"

ModbusParameterSettingsDialog::ModbusParameterSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModbusParameterSettingsDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setAttribute(Qt::WA_QuitOnClose,false);

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

ModbusParameterSettingsDialog::~ModbusParameterSettingsDialog()
{
    delete ui;
}

void ModbusParameterSettingsDialog::on_buttonBox_accepted()
{
    ModbusParameter mp;
    StModbusSerialParameter smp;
    smp.parity = ui->parityCombo->currentIndex();
    smp.baud = ui->baudCombo->currentText().toInt();
    smp.dataBits = ui->dataBitsCombo->currentText().toInt();
    smp.stopBits = ui->stopBitsCombo->currentText().toInt();
    smp.responseTime = ui->timeoutSpinner->value();
    smp.numberOfRetries = ui->retriesSpinner->value();
    mp.SaveSerialParameter(smp);
}
