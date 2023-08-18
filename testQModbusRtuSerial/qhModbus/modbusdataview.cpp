#include "modbusdataview.h"
#include "ui_modbusdataview.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include "modbusparameter.h"
#include "modbusparametersettingsdialog.h"

#define READ_SET_TEMP_INTERVAL_SEC  (10)//多久读一次设定值

#define REG_QUREY_CUR_TEMP          0x100//只允许0x03功能码读

#define REG_QUREY_SET_TEMP          0x000//可读写，允许0x03 0x06 0x10读写

static const char blankString[] = QT_TRANSLATE_NOOP("ModbusDataView", "N/A");

ModbusDataView::ModbusDataView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusDataView)
{
    ui->setupUi(this);
    ui->textEditMsg->document()->setMaximumBlockCount(10000);

    mqf.setFileName(QApplication::applicationDirPath() + "/modbus.log");
    if (mqf.open(QFile::WriteOnly | QFile::Append)) {
        mqst.setDevice(&mqf);
        mqst<<QDateTime::currentDateTime().toString()<<endl;
    }

    mReadIndex = 0;
    mReadIndex2 = 0;
    mbWaitingWriteReply = false;

    ui->widgetServer1->SetTitle("调节仪1");
    ui->widgetServer1->SetAddr(1);
    mMapAddrServer.insert(ui->widgetServer1->GetAddr(), ui->widgetServer1);
    connect(ui->widgetServer1, SIGNAL(SigSetTemp(int,float_t)), this, SLOT(OnSigManualSetTemp(int,float_t)));

    ui->widgetServer2->SetTitle("调节仪2");
    ui->widgetServer2->SetAddr(2);
    mMapAddrServer.insert(ui->widgetServer2->GetAddr(), ui->widgetServer2);
    connect(ui->widgetServer2, SIGNAL(SigSetTemp(int,float_t)), this, SLOT(OnSigManualSetTemp(int,float_t)));

    modbusDevice = new QModbusRtuSerialMaster(this);
    QModbusRtuSerialMaster *qrsm = dynamic_cast<QModbusRtuSerialMaster *>(modbusDevice);
    //TODO:5.9.8的源码qmodbusrtuserialmaster_p.h那里用这个作为发送超时和帧间隔了，默认值可能会导致发送超时，这里把它设大
    if(qrsm){
        qrsm->setInterFrameDelay(10*1000);
    }
    connect(modbusDevice, &QModbusClient::stateChanged,
            this, &ModbusDataView::onStateChanged);
    connect(modbusDevice, &QModbusClient::errorOccurred,
            this, &ModbusDataView::onErrorOccurred);

    RefreshSerialPort();

    ModbusParameter mp;
    QString name;
    mp.LoadSerialName(name);
    ui->serialPortInfoListBox->setCurrentText(name);

    mReadPeriod.setInterval(800);
    mReadPeriod.setSingleShot(true);
    connect(&mReadPeriod, SIGNAL(timeout()), this, SLOT(OnReadLoop()));
}

ModbusDataView::~ModbusDataView()
{
    if (modbusDevice){
        modbusDevice->disconnectDevice();
        delete modbusDevice;
    }

    delete ui;
}

void ModbusDataView::onErrorOccurred(QModbusDevice::Error /*error*/)
{
    ShowErrMsg(QString("通讯异常!(%1)").arg(modbusDevice->errorString()));
}

void ModbusDataView::onStateChanged(int state)
{
    if (state == QModbusDevice::UnconnectedState){
        ui->pushButtonConn->setText("打开");
        ui->pushButtonParameter->setEnabled(true);

        mReadPeriod.stop();
    }else if (state == QModbusDevice::ConnectedState){
        ui->pushButtonConn->setText("关闭");
        ui->pushButtonParameter->setEnabled(false);

        mReadPeriod.start();
        mLastReadSetTempTime = QDateTime::currentDateTime().addSecs(-READ_SET_TEMP_INTERVAL_SEC);
    }
    mReadIndex = 0;
    mReadIndex2 = 0;
    mbWaitingWriteReply = false;
}

void ModbusDataView::RefreshSerialPort()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(info.portName()+" "+description, list);
    }
}

void ModbusDataView::ShowErrMsg(const QString &err)
{
    if(err.isEmpty()){
        return;
    }
    QString sNewText = QString("[%1][ERR]%2")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(err);
    ui->textEditMsg->append(sNewText);
    qWarning()<<sNewText;

    if(mqst.device()){
        mqst<<sNewText<<endl;
    }
}

void ModbusDataView::ShowInfoMsg(const QString &info)
{
    QString sNewText = QString("[%1][INF]%2")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(info);
    ui->textEditMsg->append(sNewText);
    qInfo()<<sNewText;

    if(mqst.device()){
        mqst<<sNewText<<endl;
    }
}

bool ModbusDataView::QueryTemperature(int addr)
{
    ShowInfoMsg(QString("查询地址%1当前温度").arg(addr));
    QModbusDataUnit requset = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, REG_QUREY_CUR_TEMP, 1);
    return Read(requset, addr);
}

bool ModbusDataView::QuerySetTemperature(int addr)
{
    ShowInfoMsg(QString("查询地址%1设定温度").arg(addr));
    QModbusDataUnit requset = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, REG_QUREY_SET_TEMP, 1);
    return Read(requset, addr);
}

bool ModbusDataView::SetTemperature(int addr, float_t temp)
{
    //写的时候要停止mReadPeriod
    QString sValue = QString::number(temp, 'f', 1);
    sValue = QString::number(sValue.toDouble(), 'f', QLocale::FloatingPointShortest);
    ShowInfoMsg(QString("设置地址%1温度为%2").arg(addr).arg(sValue));
    QModbusDataUnit req = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, REG_QUREY_SET_TEMP, 1);
    uint16_t ut = temp*10;
    req.setValue(0, ut);

    bool bRet = Write(req, addr);
    if(bRet){
        mReadPeriod.stop();
    }

    return bRet;
}

bool ModbusDataView::Read(const QModbusDataUnit &req, int serverAddress)
{
    if (modbusDevice->state() != QModbusDevice::ConnectedState){
        return false;
    }
    ShowErrMsg("");

    if (auto *reply = modbusDevice->sendReadRequest(req, serverAddress)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusDataView::OnReadReply);
        else
            delete reply; // broadcast replies return immediately

        return true;
    } else {
        ShowErrMsg(QString("读失败:%1").arg(modbusDevice->errorString()));
        return false;
    }
}

bool ModbusDataView::Write(const QModbusDataUnit &req, int serverAddress)
{
    if (modbusDevice->state() != QModbusDevice::ConnectedState){
        return false;
    }
    ShowErrMsg("");

    if (auto *reply = modbusDevice->sendWriteRequest(req, serverAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, &ModbusDataView::OnWriteReply);
            mbWaitingWriteReply = true;
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
        return true;
    } else {
        ShowErrMsg(QString("写失败:").arg(modbusDevice->errorString()));
        return false;
    }
}

void ModbusDataView::on_pushButtonConn_clicked()
{
    ui->textEditMsg->clear();
    if(modbusDevice->state() != QModbusDevice::ConnectedState){
        ModbusParameter mp;
        StModbusSerialParameter smp;
        mp.LoadSerialParameter(smp);

        QString com = ui->serialPortInfoListBox->itemData(ui->serialPortInfoListBox->currentIndex()).toStringList().at(0);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
            com);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
            smp.parity);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
            smp.baud);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
            smp.dataBits);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
            smp.stopBits);

        mp.SaveSerialName(ui->serialPortInfoListBox->currentText());

        modbusDevice->setTimeout(smp.responseTime);
        modbusDevice->setNumberOfRetries(smp.numberOfRetries);

        if (!modbusDevice->connectDevice()) {
            ShowErrMsg(QString("打开失败:%1").arg(modbusDevice->errorString()));
        }
    }else{
        modbusDevice->disconnectDevice();
    }
}

void ModbusDataView::on_pushButtonParameter_clicked()
{
    ModbusParameterSettingsDialog mps;
    mps.exec();
}

void ModbusDataView::OnSigManualSetTemp(int addr, float_t t)
{
    if(mbWaitingWriteReply){
        QMessageBox::warning(this, "正在等待返回", "请稍后再试");
        return;
    }
    ShowInfoMsg("手动设置温度");
    SetTemperature(addr, t);
}

void ModbusDataView::OnReadLoop()
{
    if (modbusDevice->state() != QModbusDevice::ConnectedState){
        return;
    }

    if(mLastReadSetTempTime.secsTo(QDateTime::currentDateTime()) >= READ_SET_TEMP_INTERVAL_SEC){
        if(mReadIndex2 < mMapAddrServer.size()){
            FormTemp *ft = mMapAddrServer.values().at(mReadIndex2);
            QuerySetTemperature(ft->GetAddr());
        }

        mReadIndex2++;
        if(mReadIndex2 >= mMapAddrServer.size()){
            mReadIndex2 = 0;
            mLastReadSetTempTime = QDateTime::currentDateTime();
        }
    }else{
        if(mReadIndex < mMapAddrServer.size()){
            FormTemp *ft = mMapAddrServer.values().at(mReadIndex);
            QueryTemperature(ft->GetAddr());
        }

        mReadIndex++;
        if(mReadIndex >= mMapAddrServer.size()){
            mReadIndex = 0;
        }
    }
}

void ModbusDataView::OnReadReply()
{
    mReadPeriod.start();

    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            const QString entry = tr("serverAddress:%3, Reg: 0x%1, Value: 0x%2")
                                    .arg(QString::number(unit.startAddress() + i, 16))
                                    .arg(QString::number(unit.value(i),16))
                                    .arg(reply->serverAddress());
            ShowInfoMsg(entry);
        }

        if(REG_QUREY_CUR_TEMP == unit.startAddress()){
            OnCurTemperatureReplay(reply->serverAddress(), unit.value(0));
        }else if(REG_QUREY_SET_TEMP == unit.startAddress()){
            OnQuerySetTemperatureReplay(reply->serverAddress(), unit.value(0));
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        ShowErrMsg(tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16));
    } else {
        ShowErrMsg(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
}

void ModbusDataView::OnWriteReply()
{
    mReadPeriod.start();
    mbWaitingWriteReply = false;

    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        if(REG_QUREY_SET_TEMP == unit.startAddress()){
            ShowInfoMsg(QString("设置地址%1温度成功").arg(reply->serverAddress()));
            OnQuerySetTemperatureReplay(reply->serverAddress(), unit.value(0));
        }
    }else if (reply->error() == QModbusDevice::ProtocolError) {
        ShowErrMsg(tr("Write response error: %1 (Mobus exception: 0x%2)")
            .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
    } else if (reply->error() != QModbusDevice::NoError) {
        ShowErrMsg(tr("Write response error: %1 (code: 0x%2)").
            arg(reply->errorString()).arg(reply->error(), -1, 16));
    }else {
        ShowErrMsg(tr("Write response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
}

void ModbusDataView::OnCurTemperatureReplay(int serverAddr, uint16_t value)
{
    float_t temp = value/10.0;

    if(mMapAddrServer.contains(serverAddr)){
        FormTemp *ft = mMapAddrServer.value(serverAddr);
        ft->UpdateTemp(temp);
    }
}

void ModbusDataView::OnQuerySetTemperatureReplay(int serverAddr, uint16_t value)
{
    float_t temp = value/10.0;

    if(mMapAddrServer.contains(serverAddr)){
        FormTemp *ft = mMapAddrServer.value(serverAddr);
        ft->UpdateSetTemp(temp);
    }
}
