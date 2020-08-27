#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_save_clicked()
{
    ui->widget->Save();
}

void MainWindow::on_pushButton_load_clicked()
{
    ui->widget->Load();
}

void MainWindow::on_pushButton_test_clicked()
{
    ROLE_AUTHORITY roleAuthority;
    ui->widget->GetAuthorityByIndex(0, roleAuthority);
    qDebug()<<roleAuthority.Role;
    qDebug()<<roleAuthority.AuthorityStr;
    qDebug()<<roleAuthority.AuthorityIndex;
    qDebug()<<roleAuthority.Authority;
}
