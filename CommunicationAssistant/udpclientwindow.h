#ifndef UDPCLIENTWINDOW_H
#define UDPCLIENTWINDOW_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class UdpClientWindow;
}

class UdpClientWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UdpClientWindow(QWidget *parent = nullptr);
    ~UdpClientWindow();

private slots:
    void on_pushButton_connect_clicked();

    void OnReadyRead();
    void OnSigSend(const QByteArray &qbData);

private:
    Ui::UdpClientWindow *ui;
    QUdpSocket mUdpSocket;

    void Init();
};

#endif // UDPCLIENTWINDOW_H
