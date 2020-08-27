#ifndef MYAUTHORITY_H
#define MYAUTHORITY_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

namespace Ui {
class MyAuthority;
}

typedef struct
{
    QString Role;//角色名称
    QList<bool> Authority;//权限列表,true为有权限,false为无权限
    QList<int> AuthorityIndex;//权限索引
    QList<QString> AuthorityStr;//权限说明
}ROLE_AUTHORITY;

#ifdef TEST_WITH_SRC
class MyAuthority : public QWidget
#else
class QDESIGNER_WIDGET_EXPORT MyAuthority : public QWidget
#endif
{
    Q_OBJECT
    //用户角色列表
    Q_PROPERTY(QStringList roles READ GetRoles WRITE SetRoles)
    //最大的权限数量
    Q_PROPERTY(qint32 maxAuthority READ GetMaxAuthority WRITE SetMaxAuthority)

public:
    explicit MyAuthority(QWidget *parent = nullptr);
    ~MyAuthority();

    QStringList GetRoles();
    void SetRoles(const QStringList &roles);

    qint32 GetMaxAuthority();
    void SetMaxAuthority(qint32 maxNum);

    void retranslateUi();

    bool GetAuthorityByIndex(const int index, ROLE_AUTHORITY &roleAutority);

    bool Load();
    bool Save();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void changeEvent(QEvent *event);

private:
    Ui::MyAuthority *ui;

    bool mbInit;
    QStringList mRoles;
    qint32 mMaxAuthority;
    QStringList mAuthorityList;

    void InitUI();
};

#endif // MYAUTHORITY_H
