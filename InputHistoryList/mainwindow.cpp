#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //如果需要设置的QSpinBox或者QLineEdit很多，可以继承QSpinBox或者QLineEdit写一个新类继承历史输入
    mInputHistoryA = new InputHistoryList(ui->centralWidget);
    mInputHistoryA->SetShowUnderWidget(ui->spinBox);
    mInputHistoryA->SetHistory(QStringList()<<"1"<<"2");

    mInputHistoryB = new InputHistoryList(ui->centralWidget);
    mInputHistoryB->SetShowUnderWidget(ui->lineEdit);
    mInputHistoryB->SetHistory(QStringList()<<"a"<<"b");
}

MainWindow::~MainWindow()
{
    delete ui;
}
