#include "tcpserverwindow.h"
#include "ui_tcpserverwindow.h"
#include <QNetworkInterface>
#include <QMessageBox>
#include <QSettings>
#include "commonfunc.h"

TcpServerWindow::TcpServerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServerWindow)
{
    ui->setupUi(this);

    Init();
}

TcpServerWindow::~TcpServerWindow()
{
    SaveSendHistory();
    delete ui;
}

void TcpServerWindow::InitIP()
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

void TcpServerWindow::Init()
{
    InitIP();

    ui->listView_clients->setModel(&mClientListModel);

    LoadSetting();
    LoadSendHistroy();

    connect(&mTcpServer, SIGNAL(SigServerClientConnect(int,QString,int)), this, SLOT(OnSigServerClientConnect(int,QString,int)));
    connect(&mTcpServer, SIGNAL(SigServerClientDisConnect(int,QString,int)), this, SLOT(OnSigServerClientDisConnect(int,QString,int)));
    connect(&mTcpServer, SIGNAL(SigServerRecv(int,QString,int,QByteArray)), this, SLOT(OnSigServerRecv(int,QString,int,QByteArray)));

    connect(ui->textBrowse_send, SIGNAL(SigSend(QByteArray)), this, SLOT(OnSigSend(QByteArray)));
}

void TcpServerWindow::LoadSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    ui->spinBox_server_port->setValue(setting.value("/TcpServer/Port", 50000).toInt());
    ui->comboBox_server_ip->setCurrentText(setting.value("/TcpServer/IP", "127.0.0.1").toString());
}

void TcpServerWindow::SaveSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);

    setting.setValue("/TcpServer/IP", ui->comboBox_server_ip->currentText());
    setting.setValue("/TcpServer/Port", ui->spinBox_server_port->text());
}

void TcpServerWindow::SaveSendHistory()
{
    QStringList history = ui->textBrowse_send->GetHistory();

    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    setting.setValue("/TcpServer/History", history.join("#$"));
}

void TcpServerWindow::LoadSendHistroy()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    QString sHistory = setting.value("/TcpServer/History", "").toString();
    if(sHistory != ""){
        QStringList list = sHistory.split("#$");
        ui->textBrowse_send->AddHistory(list);
    }
}

void TcpServerWindow::on_pushButton_server_clicked()
{
    if(mTcpServer.isListening()){
        mTcpServer.StopListen();
        ui->pushButton_server->setChecked(false);
    }else{
        QString sErrMsg;
        bool bRet = mTcpServer.StartListen(sErrMsg,
                                           ui->spinBox_server_port->text().toInt(),
                                           QHostAddress(ui->comboBox_server_ip->currentText()));

        if(!bRet){
            QMessageBox::critical(this, tr("server start fail"), sErrMsg);
            ui->pushButton_server->setChecked(false);
        }else{
            ui->pushButton_server->setChecked(true);
            SaveSetting();
        }
    }

    if(ui->pushButton_server->isChecked()){
        ui->pushButton_server->setText(tr("stop"));
    }else{
        ui->pushButton_server->setText(tr("listen"));
    }
    ui->comboBox_server_ip->setEnabled(!ui->pushButton_server->isChecked());
    ui->spinBox_server_port->setEnabled(!ui->pushButton_server->isChecked());
}

void TcpServerWindow::OnSigServerRecv(int /*clientID*/, QString IP, int Port, QByteArray data)
{
    ui->textBrower_recv->Append(IP, Port, data);
}

void TcpServerWindow::OnSigServerClientConnect(int clientID, QString IP, int Port)
{
    QString sNewClient = QString("%1:%2").arg(IP).arg(Port);
    mClientMap.insert(sNewClient, clientID);
    mClientListModel.setStringList(QStringList(mClientMap.keys()));
}

void TcpServerWindow::OnSigServerClientDisConnect(int /*clientID*/, QString IP, int Port)
{
    QString sClient = QString("%1:%2").arg(IP).arg(Port);
    mClientMap.remove(sClient);
    mClientListModel.setStringList(QStringList(mClientMap.keys()));
}

void TcpServerWindow::OnSigSend(const QByteArray &qbData)
{
    QString sErrMsg;
    if(ui->listView_clients->currentIndex().isValid()){
        QString sClient = ui->listView_clients->currentIndex().data().toString();
        int clientID = mClientMap.value(sClient);
        int ret = mTcpServer.SendMsgToClient(clientID, qbData, sErrMsg);
        if(ret < 0){
            QMessageBox::warning(this, tr("send fail"), sErrMsg);
        }
    }else{
        QMap<QString, int>::const_iterator i = mClientMap.constBegin();
        while (i != mClientMap.constEnd()) {
            mTcpServer.SendMsgToClient(i.value(), qbData, sErrMsg);
            ++i;
        }
    }
}
