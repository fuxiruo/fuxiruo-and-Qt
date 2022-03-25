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
    SaveSetting();
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

    ui->textEdit_auto_reply->setText(setting.value("/TcpServer/AutoReply", "").toString());

    ui->textBrowse_send->SetAutoHead(setting.value("/TcpServer/AutoHead", "").toString());
    ui->textBrowse_send->SetAutoTail(setting.value("/TcpServer/AutoTail", "").toString());
}

void TcpServerWindow::SaveSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);

    setting.setValue("/TcpServer/IP", ui->comboBox_server_ip->currentText());
    setting.setValue("/TcpServer/Port", ui->spinBox_server_port->text());

    setting.setValue("/TcpServer/AutoReply", ui->textEdit_auto_reply->toPlainText());

    setting.setValue("/TcpServer/AutoHead", ui->textBrowse_send->GetAutoHead());
    setting.setValue("/TcpServer/AutoTail", ui->textBrowse_send->GetAutoTail());
}

void TcpServerWindow::UpdateSendRecvCount(qint32 mnAddSend, qint32 mnAddRecv)
{
    mnSend += mnAddSend;
    mnRecv += mnAddRecv;
    ui->label_count->setText(QString("TX:%1\tRX:%2").arg(mnSend).arg(mnRecv));
}

void TcpServerWindow::on_pushButton_server_clicked()
{
    if(mTcpServer.isListening()){
        mTcpServer.StopListen();
        ui->pushButton_server->setChecked(false);
        mClientMap.clear();
        mClientListModel.setStringList(QStringList(mClientMap.keys()));
    }else{
        mnSend = 0;
        mnRecv = 0;
        UpdateSendRecvCount();

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
    QString convText = ui->textBrower_recv->Append(IP, Port, data);
    AutoReply(convText);
    UpdateSendRecvCount(0, data.size());
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
            return;
        }
    }else{
        QMap<QString, int>::const_iterator i = mClientMap.constBegin();
        while (i != mClientMap.constEnd()) {
            mTcpServer.SendMsgToClient(i.value(), qbData, sErrMsg);
            ++i;
        }
    }

    UpdateSendRecvCount(qbData.size(), 0);
}

void TcpServerWindow::on_checkBox_auto_reply_toggled(bool checked)
{
    if(checked){
        mAutoReplyMap.clear();
        QStringList qsl = ui->textEdit_auto_reply->toPlainText().split("\n");
        foreach (QString sLine, qsl) {
            QStringList qslRS = sLine.split("->");
            if(2 == qslRS.size()){
                qDebug()<<"auto reply:"<<sLine;
                mAutoReplyMap.insert(qslRS.at(0), qslRS.at(1));
            }
        }
    }
}

void TcpServerWindow::AutoReply(QString sRecv)
{
    if(ui->checkBox_auto_reply->isChecked()){
        if(!ui->textEdit_auto_reply->toPlainText().isEmpty()){
            QString sKey = "";
            foreach (QString sTemp, mAutoReplyMap.keys()) {
                if(sRecv == sTemp){
                    sKey = sTemp;
                    break;
                }else{
                    QRegExp re(sTemp);
                    if(sRecv.contains(re)){
                        sKey = sTemp;
                    }
                }
            }

            if(mAutoReplyMap.contains(sKey)){
                ui->textBrowse_send->Send(mAutoReplyMap.value(sKey).toUtf8());
            }else if(mAutoReplyMap.contains("*")){
                ui->textBrowse_send->Send(mAutoReplyMap.value("*").toUtf8());
            }else{
                ui->textBrowse_send->Send(ui->textEdit_auto_reply->toPlainText().toUtf8());
            }
        }
    }
}
