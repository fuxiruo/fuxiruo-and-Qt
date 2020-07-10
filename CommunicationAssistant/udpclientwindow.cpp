#include "udpclientwindow.h"
#include "ui_udpclientwindow.h"
#include <QNetworkDatagram>

UdpClientWindow::UdpClientWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpClientWindow)
{
    ui->setupUi(this);

    Init();
}

UdpClientWindow::~UdpClientWindow()
{
    delete ui;
}

void UdpClientWindow::on_pushButton_connect_clicked()
{
    if(QAbstractSocket::ConnectedState == mUdpSocket.state()){
        mUdpSocket.close();
        ui->pushButton_connect->setChecked(false);
    }else{
        mUdpSocket.connectToHost(QHostAddress(ui->lineEdit_ip->text()), ui->spinBox_port->text().toInt());
    }

    if(ui->pushButton_connect->isChecked()){
        ui->pushButton_connect->setText(tr("stop"));
    }else{
        ui->pushButton_connect->setText(tr("connect"));
    }
    ui->lineEdit_ip->setEnabled(!ui->pushButton_connect->isChecked());
    ui->spinBox_port->setEnabled(!ui->pushButton_connect->isChecked());
}

void UdpClientWindow::OnReadyRead()
{
    if(mUdpSocket.bytesAvailable()){
        QByteArray data = mUdpSocket.readAll();
        ui->textBrower_recv->Append(mUdpSocket.peerAddress().toString(),
                                    mUdpSocket.peerPort(),
                                    data);
    }
}

void UdpClientWindow::OnSigSend(const QByteArray &qbData)
{
    mUdpSocket.write(qbData);
}

void UdpClientWindow::Init()
{
    connect(&mUdpSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));

    connect(ui->textBrowse_send, SIGNAL(SigSend(QByteArray)), this, SLOT(OnSigSend(QByteArray)));
}
