#include "udpserverwindow.h"
#include "ui_udpserverwindow.h"
#include <QNetworkInterface>
#include <QNetworkDatagram>

UdpServerWindow::UdpServerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpServerWindow)
{
    ui->setupUi(this);

    Init();
}

UdpServerWindow::~UdpServerWindow()
{
    delete ui;
}

void UdpServerWindow::InitIP()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    QList<QHostAddress> IPv4AddressesList;
    IPv4AddressesList.append(QHostAddress::LocalHost);
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            IPv4AddressesList.append(ipAddressesList.at(i));
        }
    }

    QStringList qslIP;
    QString ipAddress;
    for (int i = 0; i < IPv4AddressesList.size(); ++i) {
        ipAddress = IPv4AddressesList.at(i).toString();
        qslIP.append(ipAddress);
    }

    ui->comboBox_server_ip->addItems(qslIP);
}

void UdpServerWindow::Init()
{
    InitIP();

    ui->listView_clients->setModel(&mClientListModel);

    connect(&mUdpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    connect(ui->textBrowse_send, SIGNAL(SigSend(QByteArray)), this, SLOT(OnSigSend(QByteArray)));
}

void UdpServerWindow::AddClient(QString IP, int port)
{
    if(-1 == port){
        return;
    }

    QString sClient = QString("%1:%2").arg(IP).arg(port);
    QStringList qsl = mClientListModel.stringList();
    if(!qsl.contains(sClient)){
        qsl.append(sClient);
        mClientListModel.setStringList(qsl);
    }
}

void UdpServerWindow::on_pushButton_server_clicked()
{
    if(QAbstractSocket::BoundState == mUdpSocket.state()){
        mUdpSocket.close();
        ui->pushButton_server->setChecked(false);
        mClientListModel.setStringList(QStringList());
    }else{
        mUdpSocket.bind(QHostAddress(ui->comboBox_server_ip->currentText()), ui->spinBox_server_port->text().toInt());
    }

    if(ui->pushButton_server->isChecked()){
        ui->pushButton_server->setText(tr("stop"));
    }else{
        ui->pushButton_server->setText(tr("listen"));
    }
    ui->comboBox_server_ip->setEnabled(!ui->pushButton_server->isChecked());
    ui->spinBox_server_port->setEnabled(!ui->pushButton_server->isChecked());
}

void UdpServerWindow::readPendingDatagrams()
{
    while (mUdpSocket.hasPendingDatagrams()) {
        QNetworkDatagram datagram = mUdpSocket.receiveDatagram();
        ui->textBrower_recv->Append(datagram.senderAddress().toString(),
                                    datagram.senderPort(),
                                    datagram.data());

        AddClient(datagram.senderAddress().toString(), datagram.senderPort());
    }
}

void UdpServerWindow::OnSigSend(const QByteArray &qbData)
{
    if(ui->listView_clients->currentIndex().isValid()){
        QString sClient = ui->listView_clients->currentIndex().data().toString();
        QStringList addrPort = sClient.split(":");
        mUdpSocket.writeDatagram(qbData.data(), qbData.size(),
                                 QHostAddress(addrPort.at(0)),
                                 addrPort.at(1).toInt());
    }else{
        QStringList qsl = mClientListModel.stringList();
        foreach (QString sClient, qsl) {
            QStringList addrPort = sClient.split(":");
            mUdpSocket.writeDatagram(qbData.data(), qbData.size(),
                                     QHostAddress(addrPort.at(0)),
                                     addrPort.at(1).toInt());
        }
    }
}
