#include "formrotate.h"
#include "ui_formrotate.h"
#include <QDebug>
#include <QMouseEvent>
#include <QToolTip>
#include <QtMath>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QDateTime>
#include <QSettings>
#include <QTimer>

FormRotate::FormRotate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRotate)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose,false);

    mBaseItemsGroup = nullptr;
    mGraphicsScene = new QGraphicsScene;
    mGraphicsScene->installEventFilter(this);
    ui->graphicsView->setScene(mGraphicsScene);

    Reset();
    QTimer::singleShot(0, this, SLOT(OnInitAfterUI()));
}

FormRotate::~FormRotate()
{
    SaveSetting();
    delete ui;
}

void FormRotate::Reset()
{
    ReInitValue();

    mMapXYNotes.clear();
    if(mBaseItemsGroup){
        mGraphicsScene->destroyItemGroup(mBaseItemsGroup);
    }
    mGraphicsScene->clear();
    ui->textEdit->clear();
}

bool FormRotate::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMousePress){
        QGraphicsSceneMouseEvent *qme = static_cast<QGraphicsSceneMouseEvent *>(event);
        if(qme->button() == Qt::LeftButton){
            if(ui->graphicsView->dragMode() == QGraphicsView::NoDrag){
                ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
            }
            QString coordinate = QString("x:%1\ny:%2").arg(qme->scenePos().x()).arg(qme->scenePos().y());
            QToolTip::showText(qme->screenPos(),coordinate,this);
        }else if(qme->button() == Qt::RightButton){
            QTransform transform;
            QGraphicsItem *item = mGraphicsScene->itemAt(qme->scenePos(), transform);
            if(item){
                if(mMapXYNotes.contains(item)){
                    mGraphicsScene->removeItem(item);
                    mGraphicsScene->removeItem(mMapXYNotes[item]);
                    mMapXYNotes.remove(item);
                }
            }
        }
    }else if(event->type() == QEvent::GraphicsSceneMouseDoubleClick){
        QGraphicsSceneMouseEvent *qme = static_cast<QGraphicsSceneMouseEvent *>(event);
        if(qme->button() == Qt::LeftButton){
            QPen pen(Qt::red);
            pen.setWidth(2/mBaseScale);
            QGraphicsEllipseItem * itemEllipse = addCircle(qme->scenePos().x(), qme->scenePos().y(), 1/mBaseScale, pen);
            QFont f;
            f.setPointSize(f.pointSize()/mBaseScale);
            QString coordinate = QString("x:%1\ny:%2").arg(qme->scenePos().x()).arg(qme->scenePos().y());
            QGraphicsTextItem * itemText = mGraphicsScene->addText(coordinate, f);
            itemText->setPos(qme->scenePos());
            mMapXYNotes.insert(itemText, itemEllipse);
        }
    }else if(event->type() == QEvent::GraphicsSceneMouseRelease){
        QGraphicsSceneMouseEvent *qme = static_cast<QGraphicsSceneMouseEvent *>(event);
        if(qme->button() == Qt::LeftButton){
            if(ui->graphicsView->dragMode() != QGraphicsView::NoDrag){
                ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
            }
        }
    }else if (event->type() == QEvent::GraphicsSceneMouseMove) {
        QGraphicsSceneMouseEvent *qme = static_cast<QGraphicsSceneMouseEvent *>(event);
        ui->label_xy->setText(QString("x:%1 y:%2").arg(qme->scenePos().x()).arg(qme->scenePos().y()));
        if(mGraphicsScene->sceneRect().contains(qme->scenePos())){
            DrawCursorRef(qme->scenePos());
        }else{
            if(mCursorRefX){
                mCursorRefX->hide();
            }
            if(mCursorRefY){
                mCursorRefY->hide();
            }
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}

void FormRotate::OnInitAfterUI()
{
    LoadSetting();
}

void FormRotate::SaveSetting()
{
    QSettings setting(QApplication::applicationDirPath()+"/config.ini", QSettings::IniFormat);
    setting.setIniCodec("UTF-8");
    setting.beginGroup(this->objectName());
    setting.setValue(ui->horizontalSlider_scale->objectName(), ui->horizontalSlider_scale->value());
    QList<QSpinBox *>lsb = ui->groupBox_param->findChildren<QSpinBox *>(QString(), Qt::FindDirectChildrenOnly);
    foreach (QSpinBox *sb, lsb) {
        setting.setValue(sb->objectName(), sb->value());
    }
    setting.endGroup();
}

void FormRotate::LoadSetting()
{
    QSettings setting(QApplication::applicationDirPath()+"/config.ini", QSettings::IniFormat);
    setting.setIniCodec("UTF-8");
    setting.beginGroup(this->objectName());
    ui->horizontalSlider_scale->setValue(setting.value(ui->horizontalSlider_scale->objectName(), 0).toInt());
    ui->spinBox_scale->setValue(ui->horizontalSlider_scale->value());

    QList<QSpinBox *>lsb = ui->groupBox_param->findChildren<QSpinBox *>();
    foreach (QSpinBox *sb, lsb) {
        sb->setValue(setting.value(sb->objectName(), sb->value()).toInt());
    }
    setting.endGroup();
}

void FormRotate::ShowLog(const QString &sLog)
{
    ui->textEdit->append(QString("[%1]%2")
                            .arg(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss.zzz"))
                         .arg(sLog));
}

void FormRotate::CalcAB(qreal &a, qreal &b, const QPoint &p1, const QPoint &p2)
{
    a = 1.0 * (p2.y() - p1.y())/(p2.x() - p1.x());
    b = p1.y() - a*p1.x();
}

void FormRotate::DrawCursorRef(const QPointF &p)
{
    QPen pen(Qt::darkCyan);
    pen.setWidth(2/mBaseScale);
    qreal sx = ui->spinBox_CenterX->value()-ui->spinBox_radius->value();
    qreal ex = ui->spinBox_CenterX->value()+ui->spinBox_radius->value();
    qreal sy = ui->spinBox_CenterY->value()-ui->spinBox_radius->value();
    qreal ey = ui->spinBox_CenterY->value()+ui->spinBox_radius->value();

    if(mCursorRefX){
        mCursorRefX->setLine(sx,p.y(),ex,p.y());
        mCursorRefX->show();
    }else{
        mCursorRefX = mGraphicsScene->addLine(sx,p.y(),ex,p.y(), pen);
    }

    if(mCursorRefY){
        mCursorRefY->setLine(p.x(),sy,p.x(),ey);
        mCursorRefY->show();
    }else{
        mCursorRefY = mGraphicsScene->addLine(p.x(),sy,p.x(),ey, pen);
    }
}

void FormRotate::DrawXYAxis()
{
    QPen pen(Qt::gray);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(10/mBaseScale);
    QGraphicsLineItem *l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()-ui->spinBox_radius->value(),ui->spinBox_CenterY->value()-ui->spinBox_radius->value(),
                            ui->spinBox_CenterX->value()+ui->spinBox_radius->value(),ui->spinBox_CenterY->value()-ui->spinBox_radius->value(), pen);
    mBaseItemsGroup->addToGroup(l1);
    l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()+ui->spinBox_radius->value()-10/mBaseScale,ui->spinBox_CenterY->value()-ui->spinBox_radius->value()-10/mBaseScale,
                            ui->spinBox_CenterX->value()+ui->spinBox_radius->value(),ui->spinBox_CenterY->value()-ui->spinBox_radius->value(), pen);
    mBaseItemsGroup->addToGroup(l1);
    l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()+ui->spinBox_radius->value()-10/mBaseScale,ui->spinBox_CenterY->value()-ui->spinBox_radius->value()+10/mBaseScale,
                            ui->spinBox_CenterX->value()+ui->spinBox_radius->value(),ui->spinBox_CenterY->value()-ui->spinBox_radius->value(), pen);
    mBaseItemsGroup->addToGroup(l1);
    l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()-ui->spinBox_radius->value(),ui->spinBox_CenterY->value()-ui->spinBox_radius->value(),
                            ui->spinBox_CenterX->value()-ui->spinBox_radius->value(),ui->spinBox_CenterY->value()+ui->spinBox_radius->value(), pen);
    mBaseItemsGroup->addToGroup(l1);
    l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()-ui->spinBox_radius->value()-10/mBaseScale,ui->spinBox_CenterY->value()+ui->spinBox_radius->value()-10/mBaseScale,
                            ui->spinBox_CenterX->value()-ui->spinBox_radius->value(),ui->spinBox_CenterY->value()+ui->spinBox_radius->value(), pen);
    mBaseItemsGroup->addToGroup(l1);
    l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()-ui->spinBox_radius->value()+10/mBaseScale,ui->spinBox_CenterY->value()+ui->spinBox_radius->value()-10/mBaseScale,
                            ui->spinBox_CenterX->value()-ui->spinBox_radius->value(),ui->spinBox_CenterY->value()+ui->spinBox_radius->value(), pen);
    mBaseItemsGroup->addToGroup(l1);
}

QGraphicsEllipseItem *FormRotate::addCircle(qreal x, qreal y, qreal r, const QPen &pen, const QBrush &brush)
{
    return mGraphicsScene->addEllipse(x-r, y-r, 2*r, 2*r, pen, brush);
}

void FormRotate::DrawBase()
{
    mBaseItemsGroup = new QGraphicsItemGroup();
    mGraphicsScene->addItem(mBaseItemsGroup);
    mBaseItemsGroup->setTransformOriginPoint(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value());

    DrawXYAxis();

    QPen p;
    p.setWidth(1/mBaseScale);
    QGraphicsEllipseItem *c1 = addCircle(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value(), ui->spinBox_radius->value(), p);
    mBaseItemsGroup->addToGroup(c1);

    p.setStyle(Qt::DashLine);
    QGraphicsLineItem *l1 = mGraphicsScene->addLine(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value()-ui->spinBox_radius->value(),
                            ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value()+ui->spinBox_radius->value(),
                            p);
    mBaseItemsGroup->addToGroup(l1);
    QGraphicsLineItem *l2 = mGraphicsScene->addLine(ui->spinBox_CenterX->value()-ui->spinBox_radius->value(), ui->spinBox_CenterY->value(),
                            ui->spinBox_CenterX->value()+ui->spinBox_radius->value(), ui->spinBox_CenterY->value(),
                            p);
    mBaseItemsGroup->addToGroup(l2);
    mGraphicsScene->setSceneRect(ui->spinBox_CenterX->value()-ui->spinBox_radius->value()-100/mBaseScale,
                                 ui->spinBox_CenterY->value()-ui->spinBox_radius->value()-100/mBaseScale,
                                 2*ui->spinBox_radius->value()+200/mBaseScale,
                                 2*ui->spinBox_radius->value()+200/mBaseScale);

    QPen pen(Qt::red);
    pen.setWidth(4/mBaseScale);
    QGraphicsEllipseItem *c2 = addCircle(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value(), 2/mBaseScale, pen);
    mBaseItemsGroup->addToGroup(c2);

    BaseItemsRotate(ui->doubleSpinBox_rotate->value());
}

void FormRotate::BaseItemsRotate(qreal angle)
{
    if(mBaseItemsGroup){
        mBaseItemsGroup->setRotation(angle);
    }
}

void FormRotate::on_pushButton_center_clicked()
{
    ui->graphicsView->centerOn(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value());
}

void FormRotate::on_spinBox_scale_editingFinished()
{
    ui->horizontalSlider_scale->setValue(ui->spinBox_scale->value());
    QTransform transform;
    transform.scale(mBaseScale*(1+ui->spinBox_scale->value()/100.0), mBaseScale*(1+ui->spinBox_scale->value()/100.0));
    ui->graphicsView->setTransform(transform);
}

void FormRotate::on_horizontalSlider_scale_sliderMoved(int position)
{
    ui->spinBox_scale->setValue(position);
    QTransform transform;
    transform.scale(mBaseScale*(1+position/100.0), mBaseScale*(1+position/100.0));
    ui->graphicsView->setTransform(transform);
}

void FormRotate::on_pushButton_updateShow_clicked()
{
    Reset();

    int n =  QString::number(ui->spinBox_radius->value()).left(1).toInt();
    if(ui->spinBox_radius->value() < 10000){
        mBaseScale = 1;
    }else if(ui->spinBox_radius->value() < 100000){
        mBaseScale = 0.1;
    }else{
        QString r = QString::number(ui->spinBox_radius->value());
        int c = 1;
        for(int i=0; i<r.count(); i++){
            c *= 10;
        }
        mBaseScale = 10000.0/c;
    }
    mBaseScale = mBaseScale/n;
    ShowLog("BaseScale:"+QString::number(mBaseScale));
    QTransform transform;
    transform.scale(mBaseScale*(1+ui->horizontalSlider_scale->value()/100.0), mBaseScale*(1+ui->horizontalSlider_scale->value()/100.0));
    ui->graphicsView->setTransform(transform);
    mLinePen.setWidth(1/mBaseScale);
    //mVLinePen.setWidth(1/mBaseScale);

    DrawBase();
}

void FormRotate::ReInitValue()
{
    mCursorRefX = nullptr;
    mCursorRefY = nullptr;
    mBaseScale = 1;
    mHLineIndex = 0;
    mVLineIndex = 0;
}

void FormRotate::on_pushButton_next_HLine_clicked()
{
    if(Qt::red == mLineColor){
        mLineColor = Qt::darkGreen;
    }else{
        mLineColor = Qt::red;
    }
    mLinePen.setColor(mLineColor);

    mLinePen.setStyle(Qt::SolidLine);
    mGraphicsScene->addLine(ui->spinBox_v_line_startX->value(), ui->spinBox_h_line_startY->value(),
                            ui->spinBox_v_line_startX->value()+ui->spinBox_start_dist->value(), ui->spinBox_h_line_startY->value(), mLinePen);
    mGraphicsScene->addLine(ui->spinBox_v_line_startX->value(), ui->spinBox_h_line_endY->value(),
                            ui->spinBox_v_line_startX->value()+ui->spinBox_end_dist->value(), ui->spinBox_h_line_endY->value(), mLinePen);

    qreal a,b;
    CalcAB(a, b,
           QPoint(ui->spinBox_v_line_startX->value()+ui->spinBox_start_dist->value(), ui->spinBox_h_line_startY->value()),
           QPoint(ui->spinBox_v_line_startX->value()+ui->spinBox_end_dist->value(), ui->spinBox_h_line_endY->value()));


    int lineTopY = ui->spinBox_CenterY->value()+ui->spinBox_radius->value();
    int lineBottomY = ui->spinBox_CenterY->value()-ui->spinBox_radius->value();
    int lineTopX = (lineTopY - b)/a;
    int linebottomX = (lineBottomY - b)/a;
    mLinePen.setStyle(Qt::DashLine);
    mGraphicsScene->addLine(ui->spinBox_v_line_startX->value(), lineBottomY,
                            ui->spinBox_v_line_startX->value(), lineTopY, mLinePen);
    mGraphicsScene->addLine(linebottomX, lineBottomY, lineTopX, lineTopY, mLinePen);

    int IntersectionPointX = ui->spinBox_v_line_startX->value();
    int IntersectionPointY = a*IntersectionPointX + b;
    QPen pen(Qt::red);
    pen.setWidth(2/mBaseScale);
    QGraphicsEllipseItem * itemEllipse = addCircle(IntersectionPointX, IntersectionPointY, 1/mBaseScale, pen);
    QFont f;
    f.setPointSize(f.pointSize()/mBaseScale);
    QString coordinate = QString("x:%1\ny:%2").arg(IntersectionPointX).arg(IntersectionPointY);
    QGraphicsTextItem * itemText = mGraphicsScene->addText(coordinate, f);
    itemText->setPos(IntersectionPointX, IntersectionPointY);
    mMapXYNotes.insert(itemText, itemEllipse);

    if(ui->spinBox_CenterX->value() != ui->spinBox_v_line_startX->value()){
        IntersectionPointX = ui->spinBox_CenterX->value();
        IntersectionPointY = a*IntersectionPointX + b;
        QGraphicsEllipseItem * itemEllipse = addCircle(IntersectionPointX, IntersectionPointY, 1/mBaseScale, pen);
        QString coordinate = QString("x:%1\ny:%2").arg(IntersectionPointX).arg(IntersectionPointY);
        QGraphicsTextItem * itemText = mGraphicsScene->addText(coordinate, f);
        itemText->setPos(IntersectionPointX, IntersectionPointY);
        mMapXYNotes.insert(itemText, itemEllipse);
    }

    mHLineIndex++;
    qreal offsetAngle = -qAtan(1/a)*180/M_PI;
    ShowLog(QString("第%1组横线偏移角度:%2")
            .arg(mHLineIndex)
            .arg(offsetAngle));
    ui->doubleSpinBox_rotate->setValue(offsetAngle);
}

void FormRotate::on_horizontalSlider_rotate_sliderMoved(int position)
{
    ui->doubleSpinBox_rotate->setValue(position);
    BaseItemsRotate(position);
}

void FormRotate::on_doubleSpinBox_rotate_valueChanged(double /*arg1*/)
{
    ui->horizontalSlider_rotate->setValue(ui->doubleSpinBox_rotate->value());
    BaseItemsRotate(ui->doubleSpinBox_rotate->value());
}

void FormRotate::on_pushButton_next_VLine_clicked()
{
    if(Qt::red == mLineColor){
        mLineColor = Qt::darkGreen;
    }else{
        mLineColor = Qt::red;
    }
    mLinePen.setColor(mLineColor);

    mLinePen.setStyle(Qt::SolidLine);
    mGraphicsScene->addLine(ui->spinBox_v_line_startX->value(), ui->spinBox_h_line_startY->value(),
                            ui->spinBox_v_line_startX->value(), ui->spinBox_h_line_startY->value()+ui->spinBox_start_dist->value(), mLinePen);
    mGraphicsScene->addLine(ui->spinBox_v_line_endX->value(), ui->spinBox_h_line_startY->value(),
                            ui->spinBox_v_line_endX->value(), ui->spinBox_h_line_startY->value()+ui->spinBox_end_dist->value(), mLinePen);

    qreal a,b;
    CalcAB(a, b,
           QPoint(ui->spinBox_v_line_startX->value(), ui->spinBox_h_line_startY->value()+ui->spinBox_start_dist->value()),
           QPoint(ui->spinBox_v_line_endX->value(), ui->spinBox_h_line_startY->value()+ui->spinBox_end_dist->value()));


    int lineRightX = ui->spinBox_CenterX->value()+ui->spinBox_radius->value();
    int lineLeftX = ui->spinBox_CenterX->value()-ui->spinBox_radius->value();
    int lineRightY = a*lineRightX + b;
    int lineLeftY = a*lineLeftX + b;
    mLinePen.setStyle(Qt::DashLine);
    mGraphicsScene->addLine(lineLeftX, ui->spinBox_h_line_startY->value(),
                            lineRightX, ui->spinBox_h_line_startY->value(), mLinePen);
    mGraphicsScene->addLine(lineLeftX, lineLeftY, lineRightX, lineRightY, mLinePen);

    int IntersectionPointY = ui->spinBox_h_line_startY->value();
    int IntersectionPointX = (IntersectionPointY-b)/a;
    QPen pen(Qt::red);
    pen.setWidth(2/mBaseScale);
    QGraphicsEllipseItem * itemEllipse = addCircle(IntersectionPointX, IntersectionPointY, 1/mBaseScale, pen);
    QFont f;
    f.setPointSize(f.pointSize()/mBaseScale);
    QString coordinate = QString("x:%1\ny:%2").arg(IntersectionPointX).arg(IntersectionPointY);
    QGraphicsTextItem * itemText = mGraphicsScene->addText(coordinate, f);
    itemText->setPos(IntersectionPointX, IntersectionPointY);
    mMapXYNotes.insert(itemText, itemEllipse);

    if(ui->spinBox_CenterY->value() != ui->spinBox_h_line_startY->value()){
        IntersectionPointY = ui->spinBox_CenterY->value();
        IntersectionPointX = (IntersectionPointY-b)/a;
        QGraphicsEllipseItem * itemEllipse = addCircle(IntersectionPointX, IntersectionPointY, 1/mBaseScale, pen);
        QString coordinate = QString("x:%1\ny:%2").arg(IntersectionPointX).arg(IntersectionPointY);
        QGraphicsTextItem * itemText = mGraphicsScene->addText(coordinate, f);
        itemText->setPos(IntersectionPointX, IntersectionPointY);
        mMapXYNotes.insert(itemText, itemEllipse);
    }

    mVLineIndex++;
    qreal offsetAngle = -qAtan(1/a)*180/M_PI;
    ShowLog(QString("第%1组竖线偏移角度:%2")
            .arg(mVLineIndex)
            .arg(offsetAngle));
    ui->doubleSpinBox_rotate->setValue(offsetAngle);
}
