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
    SaveSendHistory();

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
}

void TcpClientWindow::OnReadyRead()
{
    if(mTcpSocket.bytesAvailable()){
        QByteArray data = mTcpSocket.readAll();
        ui->textBrower_recv->Append(data);
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

void TcpClientWindow::LoadSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    ui->spinBox_port->setValue(setting.value("/TcpClient/Port", 50000).toInt());
    ui->lineEdit_ip->setText(setting.value("/TcpClient/IP", "127.0.0.1").toString());
}

void TcpClientWindow::SaveSetting()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);

    setting.setValue("/TcpClient/IP", ui->lineEdit_ip->text());
    setting.setValue("/TcpClient/Port", ui->spinBox_port->text());
}

void TcpClientWindow::SaveSendHistory()
{
    QStringList history = ui->textBrowse_send->GetHistory();

    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    setting.setValue("/TcpClient/History", history.join("#$"));
}

void TcpClientWindow::LoadSendHistroy()
{
    QSettings setting(CommonFunc::GetAppPath()+"/config.ini", QSettings::IniFormat);
    QString sHistory = setting.value("/TcpClient/History", "").toString();
    if(sHistory != ""){
        QStringList list = sHistory.split("#$");
        ui->textBrowse_send->AddHistory(list);
    }
}

void TcpClientWindow::Init()
{
    LoadSetting();
    LoadSendHistroy();

    connect(&mTcpSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
    connect(&mTcpSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));

    connect(ui->textBrowse_send, SIGNAL(SigSend(QByteArray)), this, SLOT(OnSigSend(QByteArray)));
}
