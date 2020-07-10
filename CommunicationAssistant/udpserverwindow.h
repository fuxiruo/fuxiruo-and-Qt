#ifndef UDPSERVERWINDOW_H
#define UDPSERVERWINDOW_H

#include <QWidget>
#include <QUdpSocket>
#include <QStringListModel>

namespace Ui {
class UdpServerWindow;
}

class UdpServerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UdpServerWindow(QWidget *parent = nullptr);
    ~UdpServerWindow();

private slots:
    void on_pushButton_server_clicked();

    void readPendingDatagrams();
    void OnSigSend(const QByteArray &qbData);

private:
    Ui::UdpServerWindow *ui;
    QUdpSocket mUdpSocket;
    QStringListModel mClientListModel;

    void InitIP();
    void Init();

    void AddClient(QString IP, int port);
};

#endif // UDPSERVERWINDOW_H
