#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mykeyboard.h"

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
    void on_pushButton_newDialog_clicked();

    void on_pushButton_newForm_clicked();

private:
    Ui::MainWindow *ui;
    QWidget *monitor;

    myKeyBoard mKeyBoard;

    void ShowTestForm(QWidget *widget);
};

#endif // MAINWINDOW_H
