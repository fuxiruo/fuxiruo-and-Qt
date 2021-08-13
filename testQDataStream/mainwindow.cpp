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
        p.str = "one";
        p.uNum = 1;
        qp.append(p);
        p.str = "two";
        p.uNum = 2;
        qp.append(p);

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
