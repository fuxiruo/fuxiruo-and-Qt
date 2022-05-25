#ifndef FORMCIRCLE_H
#define FORMCIRCLE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class FormCircle;
}

class FormCircle : public QWidget
{
    Q_OBJECT

public:
    explicit FormCircle(QWidget *parent = nullptr);
    ~FormCircle();

    void Reset();
    bool IsEmptyCircle();
    bool CheckIfSameCircle(qint32 centerX, qint32 centerY, qint32 radius,
                           qint32 startY, qint32 startX,
                           qint32 singleHeight, qint32 singleWidth, bool bHLine);
    void Show(QWidget *centerWidget);
    void UpdateCircle(qint32 centerX, qint32 centerY, qint32 radius,
                      qint32 startY, qint32 startX,
                      qint32 singleHeight, qint32 singleWidth);

    bool DrawHLineAndGetPos(qint32 &sy, qint32 &ey);
    bool DrawVLineAndGetPos(qint32 &sx, qint32 &ex);

protected:
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void OnInitAfterUI();

    void on_pushButton_updateShow_clicked();

    void on_pushButton_next_HLine_clicked();

    void on_pushButton_next_VLine_clicked();

    void on_horizontalSlider_scale_sliderMoved(int position);

    void on_spinBox_scale_editingFinished();

    void on_pushButton_center_clicked();

    void on_pushButton_all_line_clicked();

private:
    Ui::FormCircle *ui;
    QString msObjectName;
    bool mbIsEmptyCircle;
    qreal mBaseScale;

    qint32 mnTopEdgePos;
    qint32 mnBottomEdgePos;
    qint32 mnLeftEdgePos;
    qint32 mnRightEdgePos;

    void SaveSetting();
    void LoadSetting();

    void ReInitValue();
    void ShowLog(const QString &sLog);

    void ReCalcEdge();
    void VLineAdjustByEdge(qreal &sy, qreal &ey);
    void HLineAdjustByEdge(qreal &sx, qreal &ex);

    QGraphicsScene *mGraphicsScene;
    QMap<QGraphicsItem *, QGraphicsItem *> mMapXYNotes;
    QGraphicsLineItem *mCursorRefX;
    QGraphicsLineItem *mCursorRefY;
    void DrawXYAxis(qreal w, qreal h);
    void DrawCursorRef(QPointF p);

    QGraphicsEllipseItem *addCircle(qreal x, qreal y, qreal r, const QPen &pen = QPen(), const QBrush &brush = QBrush());

    qreal mHLineStartX;
    qreal mHLineStartY;
    qint32 mHLineIndex;
    void DrawHLines();
    QPen mHLinePen;
    Qt::GlobalColor mHLineColor;
    QGraphicsLineItem * DrawHLine();

    qreal mVLineStartX;
    qreal mVLineStartY;
    qint32 mVLineIndex;
    void DrawVLines();
    QPen mVLinePen;
    Qt::GlobalColor mVLineColor;
    QGraphicsLineItem * DrawVLine();
};

#endif // FORMCIRCLE_H
