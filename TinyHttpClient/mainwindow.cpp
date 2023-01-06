#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&thc, SIGNAL(SigReply(QNetworkReply::NetworkError,QString,QString)), this, SLOT(OnSigReply(QNetworkReply::NetworkError,QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    thc.SetUrl(ui->lineEdit_url->text());
    thc.StartRequest(ui->plainTextEdit->toPlainText().trimmed());
    ui->plainTextEdit_reply->clear();
}

void MainWindow::OnSigReply(QNetworkReply::NetworkError, const QString &errorString, const QString &reply)
{
    ui->plainTextEdit_reply->clear();

    ui->plainTextEdit_reply->appendPlainText(errorString);

    ui->plainTextEdit_reply->appendPlainText(reply);
}
