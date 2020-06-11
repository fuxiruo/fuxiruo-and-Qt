#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget_scrollText->SetText("hello world ");
    ui->widget_scrollText->SetInterval(20);

    ui->widget_scrollText_2->SetText("hello world");
    ui->widget_scrollText_2->SetInterval(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}
