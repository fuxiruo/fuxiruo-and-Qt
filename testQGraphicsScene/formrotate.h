#ifndef FORMROTATE_H
#define FORMROTATE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class FormRotate;
}

class FormRotate : public QWidget
{
    Q_OBJECT

public:
    explicit FormRotate(QWidget *parent = nullptr);
    ~FormRotate();

    void Reset();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void OnInitAfterUI();

    void on_pushButton_center_clicked();

    void on_spinBox_scale_editingFinished();

    void on_horizontalSlider_scale_sliderMoved(int position);

    void on_pushButton_updateShow_clicked();

    void on_pushButton_next_HLine_clicked();

    void on_horizontalSlider_rotate_sliderMoved(int position);

    void on_doubleSpinBox_rotate_valueChanged(double arg1);

    void on_pushButton_next_VLine_clicked();

private:
    Ui::FormRotate *ui;
    QGraphicsScene *mGraphicsScene;
    QGraphicsItemGroup *mBaseItemsGroup;
    QMap<QGraphicsItem *, QGraphicsItem *> mMapXYNotes;
    QGraphicsLineItem *mCursorRefX;
    QGraphicsLineItem *mCursorRefY;

    qreal mBaseScale;
    Qt::GlobalColor mLineColor;
    QPen mLinePen;
    qint32 mHLineIndex;
    qint32 mVLineIndex;

    void ReInitValue();
    void SaveSetting();
    void LoadSetting();

    void ShowLog(const QString &sLog);
    void CalcAB(qreal &a, qreal &b,
                const QPoint &p1,
                const QPoint &p2);//y=ax+b

    void DrawCursorRef(const QPointF &p);
    void DrawXYAxis();
    QGraphicsEllipseItem *addCircle(qreal x, qreal y, qreal r, const QPen &pen = QPen(), const QBrush &brush = QBrush());
    void DrawBase();
    void BaseItemsRotate(qreal angle);
};

#endif // FORMROTATE_H
