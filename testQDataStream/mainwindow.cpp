#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "importexportparameter.h"

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

void MainWindow::on_pushButton_export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
          tr("Export As"), "", tr("Data Files (*.dat)"));

    if(!fileName.isEmpty()){
        Parameter p;
        QVector<Parameter> qp;
        for(int i=0; i<1024; i++){
            p.str = QString::number(i).rightJustified(4, '0');
            p.uNum = i;
            qp.append(p);
        }

        ImportExportParameter::Export(fileName, qp);
    }
}

void MainWindow::on_pushButton_import_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Import From"), "", tr("Data Files (*.dat)"));
    if(!fileName.isEmpty()){
        QVector<Parameter> qp;

        ImportExportParameter::Import(fileName, qp);
    }
}
