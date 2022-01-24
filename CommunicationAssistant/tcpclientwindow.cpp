#include "tcpclientwindow.h"
#include "ui_tcpclientwindow.h"
#include <QMessageBox>
#include <QSettings>
#include "commonfunc.h"

TcpClientWindow::TcpClientWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClientWindow)
{
    ui->setupUi(this);

    Init();
}

TcpClientWindow::~TcpClientWindow()
{
    SaveSetting();

    delete ui;
}

void TcpClientWindow::on_pushButton_connect_clicked()
{
    if(QAbstractSocket::ConnectedState == mTcpSocket.state()){
        mTcpSocket.close();
        ui->pushButton_connect->setChecked(false);
    }else{
        mTcpSocket.connectToHost(ui->lineEdit_ip->text(), ui->spinBox_port->text().toInt());
        bool bOK = mTcpSocket.waitForConnected(2000);
        if(bOK){
            ui->pushButton_connect->setChecked(true);
            SaveSetting();
        }else{
            QMessageBox::critical(this, tr("connect fail"), mTcpSocket.errorString());
            ui->pushButton_connect->setChecked(false);
        }
    }

    if(ui->pushButton_connect->isChecked()){
        ui->pushButton_connect->setText(tr("disconnect"));
    }else{
        ui->pushButton_connect->setText(tr("connect"));
    }
    ui->lineEdit_ip->setEnabled(!ui->pushButton_connect->isChecked());
    ui->spinBox_port->setEnabled(!ui->pushButton_connect->isChecked());
}

void TcpClientWindow::OnDisconnected()
{
    ui->pushButton_connect->setChecked(false);
    ui->pushButton_connect->setText(tr("connect"));
    ui->lineEdit_ip->setEnabled(!ui->pushButton_connect->isChecked());
    ui->spinBox_port->setEnabled(!ui->pushButton_connect->isChecked());

    if(ui->checkBox_auto_connect->isChecked()){
        mTimerAutoConnect.start(ui->spinBox_auto_connect->value());
        ui->pushButton_connect->setEnabled(false);
    }
}

void TcpClientWindow::OnConnected()
{
    qDebug()<<"OnConnected:"<<mTcpSocket.localPort()<<mTcpSocket.peerPort();
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_connect->setChecked(true);
    ui->pushButton_connect->setText(tr("disconnect"));
    ui->lineEdit_ip->setEnabled(!ui->pushButton_connect->isChecked());
    ui->spinBox_port->setEnabled(!ui->pushButton_connect->isChecked());
}

void TcpClientWindow::OnReadyRead()
{
    if(mTcpSocket.bytesAvailable()){
        QByteArray data = mTcpSocket.readAll();
        ui->textBrower_recv->Append(data);
        AutoReply(data);
    }
}

void TcpClientWindow::OnSigSend(QByteArray data)
{
    if(QAbstractSocket::ConnectedState == mTcpSocket.state()){
        mTcpSocket.write(data);
        int timeout = 1000;
        timeout += 1000*(data.size()/2000);
        if(!mTcpSocket.waitForBytesWritten(timeout)){
            QMessageBox::warning(this, tr("send fail"), mTcpSocket.errorString());
        }
    }
}

void TcpClientWindow::OnAutoConnectTimeout()
{
    if(mTcpSocket.state() == QAbstractSocket::UnconnectedState){
        mTcpSocket.abort();
        mTcpSocket.connectToHost(ui->lineEdit_ip->text(), ui->spinBox_port->text().toInt());
    }
}

void TcpClientWindow::LoadSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    ui->spinBox_port->setValue(setting.value("/TcpClient/Port", 50000).toInt());
    ui->lineEdit_ip->setText(setting.value("/TcpClient/IP", "127.0.0.1").toString());

    ui->textEdit_auto_reply->setText(setting.value("/TcpClient/AutoReply", "").toString());
}

void TcpClientWindow::SaveSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);

    setting.setValue("/TcpClient/IP", ui->lineEdit_ip->text());
    setting.setValue("/TcpClient/Port", ui->spinBox_port->text());

    setting.setValue("/TcpClient/AutoReply", ui->textEdit_auto_reply->toPlainText());
}

void TcpClientWindow::Init()
{
    LoadSetting();

    connect(&mTcpSocket, SIGNAL(connected()), this, SLOT(OnConnected()));
    connect(&mTcpSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(&mTcpSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));

    connect(ui->textBrowse_send, SIGNAL(SigSend(QByteArray)), this, SLOT(OnSigSend(QByteArray)));

    mTimerAutoConnect.setInterval(ui->spinBox_auto_connect->value());
    connect(&mTimerAutoConnect, SIGNAL(timeout()), this, SLOT(OnAutoConnectTimeout()));
}

void TcpClientWindow::AutoReply(QString sRecv)
{
    if(ui->checkBox_auto_reply->isChecked()){
        if(!ui->textEdit_auto_reply->toPlainText().isEmpty()){
            QString sKey = "";
            foreach (QString sTemp, mAutoReplyMap.keys()) {
                if(sRecv == sTemp){
                    sKey = sTemp;
                    break;
                }else{
                    QRegularExpression re(sTemp);
                    if(sRecv.contains(re)){
                        sKey = sTemp;
                    }
                }
            }

            if(mAutoReplyMap.contains(sKey)){
                OnSigSend(mAutoReplyMap.value(sKey).toUtf8());
            }else if(mAutoReplyMap.contains("*")){
                OnSigSend(mAutoReplyMap.value("*").toUtf8());
            }else{
                OnSigSend(ui->textEdit_auto_reply->toPlainText().toUtf8());
            }
        }
    }
}

void TcpClientWindow::on_checkBox_auto_connect_toggled(bool checked)
{
    if(checked){
        if(mTcpSocket.state() == QAbstractSocket::UnconnectedState){
            mTimerAutoConnect.start(ui->spinBox_auto_connect->value());
            ui->pushButton_connect->setEnabled(false);
        }
    }else{
        ui->pushButton_connect->setEnabled(true);
        mTimerAutoConnect.stop();
    }
}

void TcpClientWindow::on_checkBox_auto_reply_toggled(bool checked)
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
