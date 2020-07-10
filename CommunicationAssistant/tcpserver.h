#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include "serverclient.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    bool StartListen(QString &sErrMsg, quint16 port = 0, const QHostAddress &address = QHostAddress::AnyIPv4);
    bool StopListen();
    int SendMsgToClient(int clientID, QByteArray qbMsg, QString &sErrMsg);

signals:
    void SigServerRecv(int clientID, QString IP, int Port, QByteArray data);
    void SigServerClientConnect(int clientID, QString IP, int Port);
    void SigServerClientDisConnect(int clientID, QString IP, int Port);

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void OnSigReadData(int clientID, QString IP, int Port, QByteArray data);
    void OnSigDisConnect(int clientID, QString IP, int Port);

private:
    QMap<int, ServerClient*> mMapClient;

    void DisconnectClient();
};

#endif // TCPSERVER_H
