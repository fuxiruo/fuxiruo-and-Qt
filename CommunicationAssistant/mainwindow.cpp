#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitAction();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnAboutAction()
{
    QMessageBox::about(this, tr("About"),
                  tr("communication assistant <b>fuxiruo</b>"));
}

void MainWindow::InitAction()
{
    connect(ui->actionabout, &QAction::triggered, this, &MainWindow::OnAboutAction);
}
