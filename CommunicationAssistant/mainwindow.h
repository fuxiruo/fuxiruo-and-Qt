#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void OnAboutAction();

    void on_actionTCP_Client_triggered();

private:
    Ui::MainWindow *ui;

    quint32 mnTcpClientCount;

    void InitAction();
};

#endif // MAINWINDOW_H
