#ifndef TCPCLIENTWINDOW_H
#define TCPCLIENTWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class TcpClientWindow;
}

class TcpClientWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TcpClientWindow(QWidget *parent = nullptr);
    ~TcpClientWindow();

private slots:
    void on_pushButton_connect_clicked();

    void OnDisconnected();
    void OnConnected();
    void OnReadyRead();

    void OnSigSend(QByteArray data);

    void OnAutoConnectTimeout();

    void on_checkBox_auto_connect_toggled(bool checked);

    void on_checkBox_auto_reply_toggled(bool checked);

private:
    Ui::TcpClientWindow *ui;

    QTcpSocket mTcpSocket;
    QTimer mTimerAutoConnect;

    QMap<QString, QString> mAutoReplyMap;

    void LoadSetting();
    void SaveSetting();
    void Init();

    void AutoReply(QString sRecv);
};

#endif // TCPCLIENTWINDOW_H
