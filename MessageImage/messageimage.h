#ifndef QHMESSAGEIMAGE_H
#define QHMESSAGEIMAGE_H

#include <QWidget>
#include "errwidget.h"
#include <QQueue>
#include <QMap>
#include <QButtonGroup>
#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class qhMessageImage;
}

typedef struct REC_ERR_DEF
{
    quint32 Idx;
    QString PicturePath;
    QString Message;
}RecErrorDefine;

#ifdef TEST_WITH_SRC
class MessageImage : public QWidget
#else
class QDESIGNER_WIDGET_EXPORT qhMessageImage : public QWidget
#endif
{
    Q_OBJECT

    Q_PROPERTY(int queryInterval READ GetQueryInterval WRITE SetQueryInterval)//主动查询模组信息(为0则不查询，通过qhMessageList查询)
    Q_PROPERTY(quint32 moduleCounter READ GetModuleCounter WRITE SetModuleCounter)  //由0开始算，查询的模组数量
    Q_PROPERTY(int loopShowInterval READ GetLoopShowInterval WRITE SetLoopShowInterval)//图片切换时间ms
    Q_PROPERTY(QString dir READ Dir WRITE SetDir)

public:
    explicit MessageImage(QWidget *parent = nullptr);
    ~MessageImage();

    void SetDir(QString sDir);
    QString Dir();

    void SetLoopShowInterval(int n);
    int GetLoopShowInterval();

    void SetQueryInterval(int n);
    int GetQueryInterval();

    quint32 GetModuleCounter();
    void SetModuleCounter(quint32 v);

public Q_SLOTS:
    void ModuleData(quint8 midx, quint8 data);//提供给外部控件直接输入模组错误信息

protected:
    void leaveEvent(QEvent *event);

private Q_SLOTS:
    void OnLoopShowTimeout();
    void OnQueryTimeout();
    void OnButtonGroupClicked(int id);
    void FromReadModule(QByteArray array);

Q_SIGNALS:
    void SigModuleErrStatus(bool bHasErr);//发出信号通知外部当前是否检测到有错误

private:
    Ui::qhMessageImage *ui;

    QQueue<ERRWidget *> msIdleErrWidgets;//ERRWidget *的whatsThis用模组序号定义
    QMap<quint8, ERRWidget *>msShowErrWidgets;//模组序号,ERRWidget *
    QString msDir;
    QTimer mLoopShowTimer;
    QQueue<QPushButton *> msIdleButton;
    QButtonGroup mButtonGroup;//里面的button的id对应模组序号

    QTimer mQueryTimer;
    quint32 mModuleCount;

    QString GetPicDir();
    void UpdateModuleErr(quint8 moduleIdx, RecErrorDefine err);
    void AddErr(quint8 moduleIdx, quint8 err);
    void AddErr(quint8 moduleIdx, RecErrorDefine err);
    void AddButton(quint8 moduleIdx, RecErrorDefine err);
    void RemoveErr(quint8 moduleIdx);
    void RemoveButton(quint8 moduleIdx);
    bool IsErrCodeInList(quint32 code, quint8 &moduleIdx);
    void NotifyModuleErrStatus(bool bHasErr);
};

#endif // QHMESSAGEIMAGE_H
