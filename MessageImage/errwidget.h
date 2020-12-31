#ifndef ERRWIDGET_H
#define ERRWIDGET_H

#include <QWidget>

namespace Ui {
class ERRWidget;
}

class ERRWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ERRWidget(QWidget *parent = nullptr);
    ~ERRWidget();

    void SetBorderPic(QString sBorderPic);
    void SetErrCode(quint32 code);
    quint32 GetErrCode();
    QString GetBorderPic();
    void SetErrInfo(QString sErrInfo);
    QString GetErrInfo();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::ERRWidget *ui;

    quint32 mErrCode;
    QString msBorderPic;
    QString msErrInfo;
    Qt::AspectRatioMode mRatioMode;

    void SetBackgroundRolePic();
};

#endif // ERRWIDGET_H
