#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QSerialPortInfo>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SerialWindow::SerialWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    intValidator = new QIntValidator(0, 4000000, this);

    InitUI();

    connect(ui->baudRateBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &checkCustomBaudRatePolicy);

    connect(serial, &QSerialPort::readyRead, this, &readData);
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &handleError);

    connect(ui->textBrowse_send, SIGNAL(SigSend(QByteArray)), this, SLOT(OnSigSend(QByteArray)));
}

SerialWindow::~SerialWindow()
{
    delete ui;
}

void SerialWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SerialWindow::readData()
{
    QByteArray data = serial->readAll();
    ui->textBrower_recv->Append(serial->portName(),
                                0,
                                data);
}

void SerialWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        CloseSerial();
    }
}

void SerialWindow::OnSigSend(QByteArray data)
{
    if(serial->isOpen()){
        serial->write(data);
    }
}

void SerialWindow::InitUI()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

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

bool SerialWindow::OpenSerial()
{
    if(ui->serialPortInfoListBox->currentIndex() < 0){
        return false;
    }

    serial->setPortName(ui->serialPortInfoListBox->itemData(ui->serialPortInfoListBox->currentIndex()).toStringList().at(0));

    if (ui->baudRateBox->currentIndex() == 4) {
        serial->setBaudRate(ui->baudRateBox->currentText().toInt());
    } else {
        serial->setBaudRate(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }

    serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt()));
    serial->setParity(static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt()));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt()));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt()));

    if(serial->open(QIODevice::ReadWrite)){
        ui->parametersBox->setEnabled(false);
        ui->pushButton_open->setText("close");
        return true;
    }else{
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        return false;
    }
}

void SerialWindow::CloseSerial()
{
    if (serial->isOpen()){
        serial->close();
    }
    ui->parametersBox->setEnabled(true);
    ui->pushButton_open->setText("open");
}

void SerialWindow::on_pushButton_open_clicked()
{
    if(serial->isOpen()){
        CloseSerial();
    }else{
        OpenSerial();
    }
}
