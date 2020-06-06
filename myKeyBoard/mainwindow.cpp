#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include "testform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    monitor = ui->widget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_newDialog_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "",
                                         "",
                                         QLineEdit::Normal,
                                         "",
                                         &ok,
                                         Qt::FramelessWindowHint);
}

void MainWindow::on_pushButton_newForm_clicked()
{
    testForm *from = new testForm(ui->centralWidget);
    ShowTestForm(from);
}

void MainWindow::ShowTestForm(QWidget *widget)
{
    if(monitor != NULL){
        ui->verticalLayout_widget->removeWidget(monitor);
        delete monitor;
    }

    monitor = widget;
    ui->verticalLayout_widget->insertWidget(0, monitor);
}
