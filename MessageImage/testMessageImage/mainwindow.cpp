#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->messageImage->SetDir("Pic");
    ui->messageImage->SetLoopShowInterval(2000);
    connect(ui->messageImage, SIGNAL(SigModuleErrStatus(bool)), this, SLOT(OnSigModuleErrStatus(bool)));

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
    mTimer.start(1500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnTimeout()
{
    static int count = 0;
    ui->messageImage->ModuleData(0, count++%3);
}

void MainWindow::OnSigModuleErrStatus(bool bHasErr)
{
    qDebug()<<"MainWindow::OnSigModuleErrStatus:"<<bHasErr;
    ui->label_err_status->setText(QString("有无报错:%1").arg(bHasErr));
}

void MainWindow::on_pushButton_add_err_clicked()
{
    ui->messageImage->ModuleData(ui->lineEdit_addr->text().toShort(), ui->lineEdit_value->text().toShort());
}
