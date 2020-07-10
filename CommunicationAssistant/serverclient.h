#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ServerClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ServerClient(QObject *parent = nullptr, int socketDescriptor=0);

signals:
    void SigClientReadData(int ClientID, QString IP, int Port, QByteArray data);
    void SigClientDisConnect(int ClientID, QString IP, int Port);

public slots:

private slots:
    void OnDisconnected();
    void OnReadyRead();

private:
    int ClientID;
};

#endif // SERVERCLIENT_H
