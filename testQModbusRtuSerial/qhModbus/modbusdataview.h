#ifndef MODBUSDATAVIEW_H
#define MODBUSDATAVIEW_H

#include <QWidget>
#include <QModbusRtuSerialMaster>
#include <QTimer>
#include <QMap>
#include <QDateTime>

#include <QTextStream>
#include <QFile>

namespace Ui {
class ModbusDataView;
}

class FormTemp;

class ModbusDataView : public QWidget
{
    Q_OBJECT

public:
    explicit ModbusDataView(QWidget *parent = nullptr);
    ~ModbusDataView();

private slots:
    void onErrorOccurred(QModbusDevice::Error error);
    void onStateChanged(int state);

    void on_pushButtonConn_clicked();

    void on_pushButtonParameter_clicked();

    void OnSigManualSetTemp(int addr, float_t t);

    void OnReadLoop();
    void OnReadReply();

    void OnWriteReply();

private:
    Ui::ModbusDataView *ui;
    QMap<int, FormTemp *> mMapAddrServer;

    QModbusClient *modbusDevice;
    QTimer mReadPeriod;
    QDateTime mLastReadSetTempTime;
    bool mbWaitingWriteReply;

    QTextStream mqst;
    QFile mqf;

    int mReadIndex;
    int mReadIndex2;

    void RefreshSerialPort();
    void ShowErrMsg(const QString &err);
    void ShowInfoMsg(const QString &info);

    bool QueryTemperature(int addr);
    bool QuerySetTemperature(int addr);
    bool SetTemperature(int addr, float_t temp);
    bool Read(const QModbusDataUnit &req, int serverAddress);
    bool Write(const QModbusDataUnit &req, int serverAddress);

    void OnCurTemperatureReplay(int serverAddr, uint16_t value);
    void OnQuerySetTemperatureReplay(int serverAddr, uint16_t value);
};

#endif // MODBUSDATAVIEW_H
