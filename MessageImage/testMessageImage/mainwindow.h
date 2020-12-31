#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    void OnTimeout();

    void OnSigModuleErrStatus(bool bHasErr);

    void on_pushButton_add_err_clicked();

private:
    Ui::MainWindow *ui;

    QTimer mTimer;
};

#endif // MAINWINDOW_H
