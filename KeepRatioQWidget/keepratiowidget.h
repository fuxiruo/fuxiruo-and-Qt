#ifndef KEEPRATIOWIDGET_H
#define KEEPRATIOWIDGET_H

#include <QWidget>
#include <QMap>

class KeepRatioWidget : public QWidget
{
    typedef struct ChildWidgetRatio
    {
        float xRatio;
        float yRatio;
        float wRatio;
        float hRatio;
    }CHILD_WIDGET_RATIO;

    Q_OBJECT
public:
    explicit KeepRatioWidget(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);

private:
    QMap<QString, CHILD_WIDGET_RATIO> mChildWidgetRatio;
    bool mbInitChildWidgetRatio;
    bool mbHasAdjustRezise;

    void InitChildWidgetRatio();
    void SetChildWidgetRatio(QWidget * widget);
    void MoveParentWidget(QWidget * widget, QList<QWidget *> &widgetHasMove);
    void WidgetSetGeometry(QWidget * widget, QList<QWidget *> &widgetHasMove);
};

#endif // KEEPRATIOWIDGET_H
