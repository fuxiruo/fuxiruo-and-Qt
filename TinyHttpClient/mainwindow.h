#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tinyhttpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void OnSigReply(QNetworkReply::NetworkError, const QString &errorString, const QString &reply);

private:
    Ui::MainWindow *ui;

    TinyHttpClient thc;
};

#endif // MAINWINDOW_H
