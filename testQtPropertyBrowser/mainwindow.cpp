#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qhuiversion.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->qhuiversion->SetFileName("testQtPropertyBrowser.exe");
    ui->qhuiversion->setObjectName("jklj");
    ui->widget->SetWidgte(ui->qhuiversion);
}

MainWindow::~MainWindow()
{
    delete ui;
}
