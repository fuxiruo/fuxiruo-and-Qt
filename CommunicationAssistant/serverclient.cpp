#include "serverclient.h"
#include <QHostAddress>

ServerClient::ServerClient(QObject *parent, int socketDescriptor) : QTcpSocket(parent)
{
    setSocketDescriptor(socketDescriptor);
    ClientID = socketDescriptor;

    connect(this, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));//挂接读取数据信号
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));//关闭连接时，发送断开连接信号
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));//关闭连接时，对象自动删除
}

void ServerClient::OnDisconnected()
{
    emit SigClientDisConnect(ClientID, peerAddress().toString(), peerPort());
}

void ServerClient::OnReadyRead()
{
    if(bytesAvailable()){
        QByteArray data = readAll();
        emit SigClientReadData(ClientID, peerAddress().toString(), peerPort(), data);
    }
}
