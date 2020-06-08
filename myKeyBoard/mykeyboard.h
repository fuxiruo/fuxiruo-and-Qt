#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include <QDialog>
#include <QLinkedList>
#include <QRegExp>
#include <QMap>
#include "cappeventfilter.h"

namespace Ui {
class myKeyBoard;
}

class myKeyBoard : public QDialog
{
    Q_OBJECT

public:
    explicit myKeyBoard(QWidget *parent = nullptr);
    ~myKeyBoard();

    void Switch(bool v);
    void StaysOnTopHint(bool state);
    void SetWidgetEventFilter(QWidget *widget);
    void UpdateWidgetEvevntFilter(QWidget *newWidget);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private Q_SLOTS:
    void OnStartInstallEventFilter();
    void OnShow();
    void OnWidgetDestroy(QObject *obj);

    void OnKeyClick();

    void on_btn_close_clicked();

private:
    Ui::myKeyBoard *ui;
    QWidget *mFocusWidget; //需要软键盘输入的widget
    bool mbSwitch;
    CAppEventFilter mAppEventFilter;
    QLinkedList<QWidget *> mIntallWidgetList;

    QRegExp mRxAz;
    QRegExp mRxNumber;
    QMap<QString, Qt::Key> mKeyMap;

    void InitUI();
    void InitBtn();
    void InitKeyMap();

    void InstallWidgetEventFilter();

    bool GetSwitch();
    void AutoLocate();
    void Show();

    bool ShiftState();
    bool CapsLockState();

    void SendKeyEvent(Qt::Key keyValue, QString keyText = "");
};

#endif // MYKEYBOARD_H
