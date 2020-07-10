#ifndef TCPCLIENTWINDOW_H
#define TCPCLIENTWINDOW_H

#include <QWidget>
#include <QTcpSocket>

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
    void OnReadyRead();

    void OnSigSend(QByteArray data);

private:
    Ui::TcpClientWindow *ui;

    QTcpSocket mTcpSocket;

    void LoadSetting();
    void SaveSetting();
    void SaveSendHistory();
    void LoadSendHistroy();

    void Init();
};

#endif // TCPCLIENTWINDOW_H
