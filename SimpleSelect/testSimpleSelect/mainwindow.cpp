#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->simpleselect, SIGNAL(SigSelectChange(int, QString)), this, SLOT(OnSelectChange(int, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QStringList sList;
    for(int i=0; i<ui->comboBox->count(); i++){
        sList.append(ui->comboBox->itemText(i));
    }
    qDebug()<<sList;
    ui->simpleselect->SetValueToText(sList);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(SimpleSelect::LeftToRight == ui->simpleselect->GetDirection()){
        ui->simpleselect->SetDirection(SimpleSelect::TopToBottom);
    }else{
        ui->simpleselect->SetDirection(SimpleSelect::LeftToRight);
    }
}

void MainWindow::OnSelectChange(int value, QString sText)
{
    qDebug()<<QString("now selct %1,%2").arg(value).arg(sText);
}
