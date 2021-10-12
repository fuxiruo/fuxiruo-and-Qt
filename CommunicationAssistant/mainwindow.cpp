#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mnTcpClientCount = 1;

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

void MainWindow::on_actionTCP_Client_triggered()
{
    TcpClientWindow *new_tcp_client = new TcpClientWindow();
    mnTcpClientCount++;
    ui->tabWidget->insertTab(ui->tabWidget->currentIndex()+1, new_tcp_client, QString("TCP Client %1").arg(mnTcpClientCount));
}
