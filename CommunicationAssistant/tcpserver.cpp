#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    mMapClient.clear();
}

TcpServer::~TcpServer()
{
    StopListen();
}

bool TcpServer::StartListen(QString &sErrMsg, quint16 port, const QHostAddress &address)
{
    if(!mMapClient.isEmpty()){
        StopListen();
    }

    if (!listen(address, port)) {
        sErrMsg = errorString();
        return false;
    }else{
        return true;
    }
}

bool TcpServer::StopListen()
{
    DisconnectClient();

    close();

    mMapClient.clear();

    return true;
}

int TcpServer::SendMsgToClient(int clientID, QByteArray qbMsg, QString &sErrMsg)
{
    int ret = 0;
    QMap<int, ServerClient*>::const_iterator i = mMapClient.find(clientID);
    if (i != mMapClient.constEnd()) {
        ret = i.value()->write(qbMsg);
        i.value()->flush();
    }else{
        ret = -2;
    }
    sErrMsg = i.value()->errorString();
    return ret;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    ServerClient *client=new ServerClient(this, socketDescriptor);

    connect(client,SIGNAL(SigClientReadData(int,QString,int,QByteArray)),this,SLOT(OnSigReadData(int,QString,int,QByteArray)));
    connect(client,SIGNAL(SigClientDisConnect(int,QString,int)),this,SLOT(OnSigDisConnect(int,QString,int)));

    mMapClient.insert(socketDescriptor, client);//将新的连接添加到列表

    emit SigServerClientConnect(socketDescriptor, client->peerAddress().toString(), client->peerPort());
}

void TcpServer::OnSigReadData(int clientID, QString IP, int Port, QByteArray data)
{
    emit SigServerRecv(clientID, IP, Port, data);
}

void TcpServer::OnSigDisConnect(int clientID, QString IP, int Port)
{
    mMapClient.remove(clientID);
    emit SigServerClientDisConnect(clientID, IP, Port);
}

void TcpServer::DisconnectClient()
{
    QMapIterator<int, ServerClient*> i(mMapClient);
    while (i.hasNext()) {
        i.next();
        i.value()->disconnect();
        i.value()->disconnectFromHost();
        i.value()->deleteLater();
    }
    mMapClient.clear();
}
