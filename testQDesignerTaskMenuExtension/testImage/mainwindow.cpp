#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "myresourceviewdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MyResourceViewDialog *dia = new MyResourceViewDialog(this);
    QStringList qsl;
    qsl.append("D:/fdq/HandlerLib/Image/testImage/pic.qrc");
    dia->SetResourceFiles(qsl);
    dia->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->qhimage->SetShowType(qhImage::Repeat);
    ui->qhimage->SetPicPath(":/release/pic/happy.png");
}
