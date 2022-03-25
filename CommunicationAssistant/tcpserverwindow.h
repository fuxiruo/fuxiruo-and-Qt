#ifndef TCPSERVER_WINDOW_H
#define TCPSERVER_WINDOW_H

#include <QWidget>
#include <QStringListModel>
#include <QMap>
#include "tcpserver.h"

namespace Ui {
class TcpServerWindow;
}

class TcpServerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServerWindow(QWidget *parent = nullptr);
    ~TcpServerWindow();

private slots:
    void on_pushButton_server_clicked();

    void OnSigServerRecv(int clientID, QString IP, int Port, QByteArray data);
    void OnSigServerClientConnect(int clientID, QString IP, int Port);
    void OnSigServerClientDisConnect(int clientID, QString IP, int Port);

    void OnSigSend(const QByteArray &qbData);

    void on_checkBox_auto_reply_toggled(bool checked);

private:
    Ui::TcpServerWindow *ui;

    TcpServer mTcpServer;
    QMap<QString, int> mClientMap;
    QStringListModel mClientListModel;
    quint32 mnSend,mnRecv;
    QMap<QString, QString> mAutoReplyMap;

    void InitIP();
    void Init();

    void LoadSetting();
    void SaveSetting();
    void SaveSendHistory();
    void LoadSendHistroy();

    void UpdateSendRecvCount(qint32 mnAddSend=0, qint32 mnAddRecv=0);

    void AutoReply(QString sRecv);
};

#endif // TCPSERVER_H
