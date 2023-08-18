#ifndef MODBUSPARAMETER_H
#define MODBUSPARAMETER_H

#include <QSettings>
#include <QSerialPort>

struct StModbusSerialParameter {
    //QString com;
    int parity = QSerialPort::NoParity;
    int baud = QSerialPort::Baud9600;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
    int responseTime = 1000;
    int numberOfRetries = 1;
};

class ModbusParameter
{
public:
    ModbusParameter();

    void SaveSerialParameter(const StModbusSerialParameter &smp);
    void LoadSerialParameter(StModbusSerialParameter &smp);

    void SaveSerialName(const QString &name);
    void LoadSerialName(QString &name);

    void SaveSetTemp(int addr, float_t t);
    void LoadSetTemp(int addr, float_t &t);

private:
    QString msParameterFile;
    QSettings mSetting;
};

#endif // MODBUSPARAMETER_H
