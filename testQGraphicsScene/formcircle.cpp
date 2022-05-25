#include "formcircle.h"
#include "ui_formcircle.h"
#include <QDebug>
#include <QMouseEvent>
#include <QToolTip>
#include <QtMath>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QDateTime>
#include <QSettings>
#include <QTimer>

FormCircle::FormCircle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCircle)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);

    ReInitValue();
    //LoadSetting();//这个函数用到了this->objectName(),但如果是通过提升方式放在界面的，它的objectName会在这个初始化函数执行完毕之后才被设置
    QTimer::singleShot(0, this, SLOT(OnInitAfterUI()));

    mGraphicsScene = new QGraphicsScene;
    mGraphicsScene->installEventFilter(this);
    ui->graphicsView->setScene(mGraphicsScene);
}

FormCircle::~FormCircle()
{
    SaveSetting();
    delete ui;
}

void FormCircle::Reset()
{
    ReInitValue();
    mMapXYNotes.clear();
    mGraphicsScene->clear();
    ui->textEdit->clear();
}

bool FormCircle::IsEmptyCircle()
{
    return mbIsEmptyCircle;
}

bool FormCircle::CheckIfSameCircle(qint32 centerX, qint32 centerY, qint32 radius, qint32 startY, qint32 startX, qint32 singleHeight, qint32 singleWidth, bool bHLine)
{
    if(centerX != ui->spinBox_CenterX->value()){
        return false;
    }
    if(centerY != ui->spinBox_CenterY->value()){
        return false;
    }
    if(radius != ui->spinBox_radius->value()){
        return false;
    }
    if(bHLine && startY != mHLineStartY){
        qWarning()<<"startY != mHLineStartY"<<startY<<mHLineStartY;
        ui->spinBox_H_line_startY->setValue(startY);
        mHLineStartY = startY;
        mHLineStartX = startX;
        //return false;
    }
    if(!bHLine && startX != mVLineStartX){
        qWarning()<<"startX != mVLineStartX"<<startX<<mVLineStartX;
        ui->spinBox_V_line_startX->setValue(startX);
        mVLineStartX = startX;
        mVLineStartY = startY;
        //return false;
    }
    if(singleHeight != ui->spinBox_height->value()){
        return false;
    }
    if(singleWidth != ui->spinBox_width->value()){
        return false;
    }

    return true;
}

void FormCircle::ReInitValue()
{
    mHLineStartX = -1;
    mHLineStartY = -1;
    mHLineIndex = 0;
    mVLineStartX = -1;
    mVLineStartY = -1;
    mVLineIndex = 0;
    mCursorRefX = nullptr;
    mCursorRefY = nullptr;
    mBaseScale = 1;
    mbIsEmptyCircle = true;
}

void FormCircle::Show(QWidget *centerWidget)
{
    if(centerWidget){
        int windowX = centerWidget->width();
        int windowY = centerWidget->height();
        int wndX =  this->width();
        int wndY = this->height();
        QPoint movePoint(windowX/2-wndX/2,windowY/2-wndY/2);
        this->move(centerWidget->mapToGlobal(movePoint));
    }
    this->show();
}

void FormCircle::UpdateCircle(qint32 centerX, qint32 centerY, qint32 radius,
                              qint32 startY, qint32 startX,
                              qint32 singleHeight, qint32 singleWidth)
{
    ui->groupBox_param->setChecked(false);
    ui->groupBox_btn->setChecked(false);

    ui->spinBox_CenterX->setValue(centerX);
    ui->spinBox_CenterY->setValue(centerY);
    ui->spinBox_radius->setValue(radius);
    ui->spinBox_H_line_startY->setValue(startY);
    ui->spinBox_V_line_startX->setValue(startX);
    ui->spinBox_height->setValue(singleHeight);
    ui->spinBox_width->setValue(singleWidth);

    on_pushButton_updateShow_clicked();
    //on_pushButton_updateShow_clicked会ReInitValue
    mHLineStartY = ui->spinBox_H_line_startY->value();
    mVLineStartX = ui->spinBox_V_line_startX->value();
}


bool FormCircle::DrawHLineAndGetPos(qint32 &sy, qint32 &ey)
{
    QGraphicsLineItem * pLine = DrawHLine();
    if(pLine){
        sy = qCeil(pLine->line().x1());
        ey = qCeil(pLine->line().x2());
        return true;
    }else{
        return false;
    }
}

bool FormCircle::DrawVLineAndGetPos(qint32 &sx, qint32 &ex)
{
    QGraphicsLineItem * pLine = DrawVLine();
    if(pLine){
        sx = qCeil(pLine->line().y1());
        ex = qCeil(pLine->line().y2());
        return true;
    }else{
        return false;
    }
}

void FormCircle::mousePressEvent(QMouseEvent *event)
{
//    QPoint pGraphicsView;
//    //globalPos坐标转为ui->graphicsView的坐标
//    pGraphicsView = ui->graphicsView->mapFromGlobal(event->globalPos());
//    //ui->graphicsView的坐标转为scene的坐标
//    QPointF pPoint = ui->graphicsView->mapToScene(pGraphicsView);

    QWidget::mousePressEvent(event);
}

bool FormCircle::eventFilter(QObject *obj, QEvent *event)
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

void FormCircle::OnInitAfterUI()
{
    LoadSetting();
}

void FormCircle::on_pushButton_updateShow_clicked()
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
    mHLinePen.setWidth(1/mBaseScale);
    mVLinePen.setWidth(1/mBaseScale);
    //DrawXYAxis(2*ui->spinBox_radius->value()+10, 2*ui->spinBox_radius->value()+10);

    QPen p;
    p.setWidth(1/mBaseScale);
    addCircle(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value(), ui->spinBox_radius->value(), p);
    p.setStyle(Qt::DashLine);
    mGraphicsScene->addLine(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value()-ui->spinBox_radius->value(),
                            ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value()+ui->spinBox_radius->value(),
                            p);
    mGraphicsScene->addLine(ui->spinBox_CenterX->value()-ui->spinBox_radius->value(), ui->spinBox_CenterY->value(),
                            ui->spinBox_CenterX->value()+ui->spinBox_radius->value(), ui->spinBox_CenterY->value(),
                            p);
    mGraphicsScene->setSceneRect(ui->spinBox_CenterX->value()-ui->spinBox_radius->value()-100/mBaseScale,
                                 ui->spinBox_CenterY->value()-ui->spinBox_radius->value()-100/mBaseScale,
                                 2*ui->spinBox_radius->value()+200/mBaseScale,
                                 2*ui->spinBox_radius->value()+200/mBaseScale);

    QPen pen(Qt::red);
    pen.setWidth(4/mBaseScale);
    addCircle(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value(), 2/mBaseScale, pen);

    mbIsEmptyCircle = false;
    on_pushButton_center_clicked();
//    DrawHLines();
//    DrawVLines();
    ReCalcEdge();
}

QGraphicsEllipseItem *FormCircle::addCircle(qreal x, qreal y, qreal r, const QPen &pen, const QBrush &brush)
{
    return mGraphicsScene->addEllipse(x-r, y-r, 2*r, 2*r, pen, brush);
}

void FormCircle::DrawHLines()
{
    qreal y = ui->spinBox_H_line_startY->value();

    int h = qAbs(ui->spinBox_CenterY->value()-y);
    qreal dx = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
    qreal sx = ui->spinBox_CenterX->value() - dx;
    qreal ex = 0;
    int index = 0;
    while(true){
        if(qAbs(ui->spinBox_CenterY->value()-y) >= ui->spinBox_radius->value()){//超出圆范围
            break;
        }

        qreal dx = ui->spinBox_CenterX->value() - sx;
        ex = ui->spinBox_CenterX->value() + dx;

        HLineAdjustByEdge(sx, ex);
        mGraphicsScene->addLine(sx, y, ex, y, mHLinePen);
        index++;
        ShowLog(QString("第%1横线:(%2,%3)->(%4,%5)")
                .arg(index)
                .arg(sx).arg(y)
                .arg(ex).arg(y));

        y += ui->spinBox_height->value();
        h = qAbs(ui->spinBox_CenterY->value()-y);
        dx = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
        if(ex>ui->spinBox_CenterX->value()){
            sx = ui->spinBox_CenterX->value() + dx;
        }else{
            sx = ui->spinBox_CenterX->value() - dx;
        }
    }
}

QGraphicsLineItem *FormCircle::DrawHLine()
{
    QGraphicsLineItem *item = nullptr;

    if(mHLineIndex == 0){//第一条线
        mHLineStartY = ui->spinBox_H_line_startY->value();
    }
    if(qAbs(ui->spinBox_CenterY->value()-mHLineStartY) >= ui->spinBox_radius->value()){//超出圆范围
        ShowLog("DrawHLine end");
        return nullptr;
    }

    int h = qAbs(ui->spinBox_CenterY->value()-mHLineStartY);
    qreal dx = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);

    qreal ex;
    if(mHLineStartX < ui->spinBox_CenterX->value()){
        mHLineStartX = ui->spinBox_CenterX->value() - dx;
        ex = ui->spinBox_CenterX->value() + dx;
    }else{
        mHLineStartX = ui->spinBox_CenterX->value() + dx;
        ex = ui->spinBox_CenterX->value() - dx;
    }

    if(Qt::red == mHLineColor){
        mHLineColor = Qt::darkGreen;
    }else{
        mHLineColor = Qt::red;
    }
    mHLinePen.setColor(mHLineColor);
    HLineAdjustByEdge(mHLineStartX, ex);
    item = mGraphicsScene->addLine(mHLineStartX, mHLineStartY, ex, mHLineStartY, mHLinePen);
    mHLineIndex++;
    ShowLog(QString("第%1横线:(%2,%3)->(%4,%5)")
            .arg(mHLineIndex)
            .arg(mHLineStartX).arg(mHLineStartY)
            .arg(ex).arg(mHLineStartY));

    mHLineStartY += ui->spinBox_height->value();
    mHLineStartX = ex;

    return item;
}

void FormCircle::DrawVLines()
{
    qreal x = ui->spinBox_V_line_startX->value();

    int h = qAbs(ui->spinBox_CenterX->value()-x);
    qreal dy = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
    qreal sy = ui->spinBox_CenterY->value() - dy;
    qreal ey = 0;
    int index = 0;
    while(true){
        if(qAbs(ui->spinBox_CenterX->value()-x) >= ui->spinBox_radius->value()){//超出圆范围
            break;
        }

        qreal dy = ui->spinBox_CenterY->value() - sy;
        ey = ui->spinBox_CenterY->value() + dy;

        VLineAdjustByEdge(sy, ey);
        mGraphicsScene->addLine(x, sy, x, ey, mVLinePen);
        index++;
        ShowLog(QString("第%1竖线:(%2,%3)->(%4,%5)")
                .arg(index)
                .arg(x).arg(sy)
                .arg(x).arg(ey));

        x += ui->spinBox_width->value();
        h = qAbs(ui->spinBox_CenterX->value()-x);
        dy = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
        if(ey>ui->spinBox_CenterY->value()){
            sy = ui->spinBox_CenterY->value() + dy;
        }else{
            sy = ui->spinBox_CenterY->value() - dy;
        }
    }
}

QGraphicsLineItem *FormCircle::DrawVLine()
{
    QGraphicsLineItem *item = nullptr;

    if(mVLineIndex == 0){//第一条线
        mVLineStartX = ui->spinBox_V_line_startX->value();
    }
    if(qAbs(ui->spinBox_CenterX->value()-mVLineStartX) >= ui->spinBox_radius->value()){//超出圆范围
        ShowLog("DrawVLine end");
        return nullptr;
    }

    int h = qAbs(ui->spinBox_CenterX->value()-mVLineStartX);
    qreal dy = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);

    qreal ey;
    if(mVLineStartY < ui->spinBox_CenterY->value()){
        mVLineStartY = ui->spinBox_CenterY->value() - dy;
        ey = ui->spinBox_CenterY->value() + dy;
    }else{
        mVLineStartY = ui->spinBox_CenterY->value() + dy;
        ey = ui->spinBox_CenterY->value() - dy;
    }

    if(Qt::red == mVLineColor){
        mVLineColor = Qt::darkGreen;
    }else{
        mVLineColor = Qt::red;
    }
    mVLinePen.setColor(mVLineColor);
    VLineAdjustByEdge(mVLineStartY, ey);
    item = mGraphicsScene->addLine(mVLineStartX, mVLineStartY, mVLineStartX, ey, mVLinePen);
    mVLineIndex++;
    ShowLog(QString("第%1竖线:(%2,%3)->(%4,%5)")
            .arg(mVLineIndex)
            .arg(mVLineStartX).arg(mVLineStartY)
            .arg(mVLineStartX).arg(ey));

    mVLineStartX += ui->spinBox_width->value();
    mVLineStartY = ey;

    return item;
}

void FormCircle::on_pushButton_next_HLine_clicked()
{
    DrawHLine();
}

void FormCircle::on_pushButton_next_VLine_clicked()
{
    DrawVLine();
}

void FormCircle::on_horizontalSlider_scale_sliderMoved(int position)
{
    ui->spinBox_scale->setValue(position);
    QTransform transform;
    transform.scale(mBaseScale*(1+position/100.0), mBaseScale*(1+position/100.0));
    ui->graphicsView->setTransform(transform);
}

void FormCircle::on_spinBox_scale_editingFinished()
{
    ui->horizontalSlider_scale->setValue(ui->spinBox_scale->value());
    QTransform transform;
    transform.scale(mBaseScale*(1+ui->spinBox_scale->value()/100.0), mBaseScale*(1+ui->spinBox_scale->value()/100.0));
    ui->graphicsView->setTransform(transform);
}

void FormCircle::on_pushButton_center_clicked()
{
    ui->graphicsView->centerOn(ui->spinBox_CenterX->value(), ui->spinBox_CenterY->value());
}

void FormCircle::SaveSetting()
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

void FormCircle::LoadSetting()
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

void FormCircle::ShowLog(const QString &sLog)
{
    ui->textEdit->append(QString("[%1]%2")
                            .arg(QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss.zzz"))
                            .arg(sLog));
}

void FormCircle::ReCalcEdge()
{
    mnTopEdgePos = ui->spinBox_CenterY->value() + ui->spinBox_radius->value() - ui->spinBox_top_margin->value();
    mnBottomEdgePos = ui->spinBox_CenterY->value() - ui->spinBox_radius->value() + ui->spinBox_bottom_margin->value();
    mnLeftEdgePos = ui->spinBox_CenterX->value() - ui->spinBox_radius->value() + ui->spinBox_left_margin->value();
    mnRightEdgePos = ui->spinBox_CenterX->value() + ui->spinBox_radius->value() - ui->spinBox_right_margin->value();

    QPen p;
    p.setWidth(10/mBaseScale);
    p.setStyle(Qt::DashLine);
    p.setColor(Qt::red);
    if(ui->spinBox_top_margin->value() > 0){
        int h = qAbs(ui->spinBox_CenterY->value()-mnTopEdgePos);
        qreal dx = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
        qreal sx = ui->spinBox_CenterX->value() - dx;
        qreal ex = ui->spinBox_CenterX->value() + dx;
        mGraphicsScene->addLine(sx, mnTopEdgePos, ex, mnTopEdgePos, p);
    }

    if(ui->spinBox_bottom_margin->value() > 0){
        int h = qAbs(ui->spinBox_CenterY->value()-mnBottomEdgePos);
        qreal dx = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
        qreal sx = ui->spinBox_CenterX->value() - dx;
        qreal ex = ui->spinBox_CenterX->value() + dx;
        mGraphicsScene->addLine(sx, mnBottomEdgePos, ex, mnBottomEdgePos, p);
    }

    if(ui->spinBox_left_margin->value() > 0){
        int h = qAbs(ui->spinBox_CenterX->value()-mnLeftEdgePos);
        qreal dy = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
        qreal sy = ui->spinBox_CenterY->value() - dy;
        qreal ey = ui->spinBox_CenterY->value() + dy;
        mGraphicsScene->addLine(mnLeftEdgePos, sy, mnLeftEdgePos, ey, p);
    }

    if(ui->spinBox_right_margin->value() > 0){
        int h = qAbs(ui->spinBox_CenterX->value()-mnRightEdgePos);
        qreal dy = qSqrt((qreal)ui->spinBox_radius->value()*ui->spinBox_radius->value()-(qreal)h*h);
        qreal sy = ui->spinBox_CenterY->value() - dy;
        qreal ey = ui->spinBox_CenterY->value() + dy;
        mGraphicsScene->addLine(mnRightEdgePos, sy, mnRightEdgePos, ey, p);
    }
}

void FormCircle::VLineAdjustByEdge(qreal &sy, qreal &ey)
{
    if(sy < ey){
        if(sy < mnBottomEdgePos){
            sy = mnBottomEdgePos;
        }
        if(ey > mnTopEdgePos){
            ey = mnTopEdgePos;
        }
    }else{
        if(sy > mnTopEdgePos){
            sy = mnTopEdgePos;
        }
        if(ey < mnBottomEdgePos){
            ey = mnBottomEdgePos;
        }
    }
}

void FormCircle::HLineAdjustByEdge(qreal &sx, qreal &ex)
{
    if(sx < ex){
        if(sx < mnLeftEdgePos){
            sx = mnLeftEdgePos;
        }
        if(ex > mnRightEdgePos){
            ex = mnRightEdgePos;
        }
    }else{
        if(sx > mnRightEdgePos){
            sx = mnRightEdgePos;
        }
        if(ex < mnLeftEdgePos){
            ex = mnLeftEdgePos;
        }
    }
}

void FormCircle::DrawXYAxis(qreal w, qreal h)
{
    QPen pen(Qt::darkBlue);
    mGraphicsScene->addLine(0,0,w,0, pen);
    mGraphicsScene->addLine(w,0,w-5,-5, pen);
    mGraphicsScene->addLine(w,0,w-5,5, pen);

    mGraphicsScene->addLine(0,0,0,h, pen);
    mGraphicsScene->addLine(-5,h-5,0,h, pen);
    mGraphicsScene->addLine(5,h-5,0,h, pen);
}

void FormCircle::DrawCursorRef(QPointF p)
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

void FormCircle::on_pushButton_all_line_clicked()
{
    DrawHLines();
    DrawVLines();
}
