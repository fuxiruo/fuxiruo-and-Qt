#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QSerialPort>
#include <QIntValidator>

namespace Ui {
class SerialWindow;
}

class SerialWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SerialWindow(QWidget *parent = nullptr);
    ~SerialWindow();

private slots:
    void checkCustomBaudRatePolicy(int idx);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void OnSigSend(QByteArray data);

    void on_pushButton_open_clicked();

private:
    Ui::SerialWindow *ui;
    QSerialPort *serial;
    QIntValidator *intValidator;

    void InitUI();
    bool OpenSerial();
    void CloseSerial();
};

#endif // SERIALWINDOW_H
