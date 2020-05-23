#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mCalendarWidget = new QCalendarWidget(this);
    mCalendarWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QWidget *pWidget = dynamic_cast<QWidget *>(QObject::sender());\
    if(pWidget){
        showCalendarUnderButton(pWidget);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QWidget *pWidget = dynamic_cast<QWidget *>(QObject::sender());
    if(pWidget){
        showCalendarUnderButton(pWidget);
    }
}

/**
 * @brief MainWindow::showCalendarUnderButton
 * 将日历显示在按钮的正下方
 */
void MainWindow::showCalendarUnderButton(QWidget *pWidget)
{
    //1.先获取按钮左上角的物理坐标(此坐标是相对我们屏幕的坐标)
    QPoint showPoint = pWidget->mapToGlobal(QPoint(0,0));

    //2.我们目标是在按钮下方显示，所以再取按钮下方的坐标
    showPoint.setY(showPoint.y() + pWidget->height());

    //3.因为setGeometry和move中的x,y都是相对于控件所在的parentWidget而言的，所以还需要将屏幕坐标转化为parentWidget的坐标
    if(mCalendarWidget->parentWidget()){
        showPoint = mCalendarWidget->parentWidget()->mapFromGlobal(showPoint);
    }

    mCalendarWidget->setGeometry(showPoint.x(), showPoint.y(), 300, 300);
    mCalendarWidget->show();
}
