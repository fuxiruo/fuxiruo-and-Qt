#include "modbusparameter.h"
#include <QApplication>

ModbusParameter::ModbusParameter()
{
    msParameterFile = QApplication::applicationDirPath() + "/" + "ModbusParameter.ini";
}

void ModbusParameter::SaveSerialParameter(const StModbusSerialParameter &smp)
{
    QSettings setting(msParameterFile, QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    setting.beginGroup("Serial");

    //setting.setValue("com", smp.com);
    setting.setValue("parity", smp.parity);
    setting.setValue("baud", smp.baud);
    setting.setValue("dataBits", smp.dataBits);
    setting.setValue("stopBits", smp.stopBits);
    setting.setValue("responseTime", smp.responseTime);
    setting.setValue("numberOfRetries", smp.numberOfRetries);

    setting.endGroup();
}

void ModbusParameter::LoadSerialParameter(StModbusSerialParameter &smp)
{
    QSettings setting(msParameterFile, QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    setting.beginGroup("Serial");

    //smp.com = setting.value("com", "COM1").toInt();
    smp.parity = setting.value("parity", QSerialPort::NoParity).toInt();
    smp.baud = setting.value("baud", QSerialPort::Baud9600).toInt();
    smp.dataBits = setting.value("dataBits", QSerialPort::Data8).toInt();
    smp.stopBits = setting.value("stopBits", QSerialPort::OneStop).toInt();
    smp.responseTime = setting.value("responseTime", 1000).toInt();
    smp.numberOfRetries = setting.value("numberOfRetries", 1).toInt();

    setting.endGroup();
}

void ModbusParameter::SaveSerialName(const QString &name)
{
    QSettings setting(msParameterFile, QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    setting.beginGroup("Serial");

    setting.setValue("com", name);

    setting.endGroup();
}

void ModbusParameter::LoadSerialName(QString &name)
{
    QSettings setting(msParameterFile, QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    setting.beginGroup("Serial");

    name = setting.value("com", "COM1").toString();

    setting.endGroup();
}

void ModbusParameter::SaveSetTemp(int addr, float_t t)
{
    QSettings setting(msParameterFile, QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    setting.beginGroup("Temperature");

    QString sValue = QString::number(t, 'f', 1);
    sValue = QString::number(sValue.toDouble(), 'f', QLocale::FloatingPointShortest);
    setting.setValue(QString::number(addr), sValue);

    setting.endGroup();
}

void ModbusParameter::LoadSetTemp(int addr, float_t &t)
{
    QSettings setting(msParameterFile, QSettings::IniFormat);
    setting.setIniCodec("utf-8");

    setting.beginGroup("Temperature");

    t = setting.value(QString::number(addr), -1).toFloat();

    setting.endGroup();
}
