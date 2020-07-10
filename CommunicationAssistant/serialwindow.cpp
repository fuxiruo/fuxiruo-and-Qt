#include "serialwindow.h"
#include "ui_serialwindow.h"

SerialWindow::SerialWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialWindow)
{
    ui->setupUi(this);
}

SerialWindow::~SerialWindow()
{
    delete ui;
}
