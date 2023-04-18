#include "formgrid.h"
#include "ui_formgrid.h"
#include <QDebug>
#include <QPushButton>

FormGrid::FormGrid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormGrid)
{
    ui->setupUi(this);

    SetRowCol(20,20);
    SetValue(10, 10, 1);
}

FormGrid::~FormGrid()
{
    delete ui;
}

void FormGrid::SetRowCol(int row, int col)
{
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            QPushButton *pushButton = new QPushButton(this);
            pushButton->setObjectName(QString("pushButton_%1_%2").arg(i).arg(j));
            pushButton->setMinimumSize(QSize(10, 10));
            pushButton->setMaximumSize(QSize(10, 10));
            pushButton->setProperty("row", i);
            pushButton->setProperty("col", j);
            QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
            pushButton->setSizePolicy(sizePolicy);

            connect(pushButton, SIGNAL(clicked(bool)), this, SLOT(OnClicked(bool)));

            ui->gridLayout->addWidget(pushButton, i, j, 1, 1);
        }
    }
}

void FormGrid::SetValue(int row, int col, int v)
{
    QWidget *w = GetWidget(row, col);

    if(w){
        QPushButton *btn = static_cast<QPushButton *>(w);

        btn->setProperty("v", v);
        btn->style()->polish(btn);
    }
}

void FormGrid::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ResizeItems();
}

void FormGrid::OnClicked(bool)
{
    static int v = 0;
    QPushButton *btn = static_cast<QPushButton *>(this->sender());

    btn->setText(QString::number(v));
    btn->setProperty("v", v);
    btn->style()->polish(btn);

    if(v == 0){
        v = 1;
    }else{
        v = 0;
    }
}

QWidget *FormGrid::GetWidget(int row, int col)
{
    QWidget *w = nullptr;
    QLayoutItem *l = ui->gridLayout->itemAtPosition(row, col);
    if(l){
        w = l->widget();
    }

    return w;
}

void FormGrid::ResizeItems()
{
    int w = this->width();
    int h = this->height();

    int maxh = h/ui->gridLayout->rowCount()-ui->gridLayout->spacing();
    int maxw = w/ui->gridLayout->columnCount()-ui->gridLayout->spacing();
    //qDebug()<<__FUNCTION__<<ui->gridLayout->spacing()<<this->width()<<maxw*ui->gridLayout->columnCount();
    for(int i=0; i<ui->gridLayout->rowCount(); i++){
        for(int j=0; j<ui->gridLayout->columnCount(); j++){
            QWidget *w = GetWidget(i, j);
            if(w){
                w->setMaximumSize(maxw, maxh);
            }
        }
    }
}
