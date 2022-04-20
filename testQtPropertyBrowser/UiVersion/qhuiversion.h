#ifndef QHUIVERSION_H
#define QHUIVERSION_H

#include <QLabel>
#include <QtUiPlugin/QDesignerExportWidget>

#ifdef TEST_WITH_SRC
class qhUiVersion : public QLabel
#else
class QDESIGNER_WIDGET_EXPORT qhUiVersion : public QLabel
#endif
{
    Q_OBJECT
    Q_PROPERTY(QString InitText READ GetInitText WRITE SetInitText)

    //显示哪个文件的版本号，为空则默认显示.exe的版本号,否则显示QCoreApplication::applicationDirPath()+fileName的版本号
    Q_PROPERTY(QString fileName READ GetFileName WRITE SetFileName)

    Q_PROPERTY(bool ProductVersion READ GetProductVersion WRITE SetProductVersion)//是否同时显示产品版本，默认显示

public:
    explicit qhUiVersion(QWidget *parent = nullptr);
    ~qhUiVersion();

    QString GetInitText();
    void SetInitText(QString sText);

    QString GetFileName();
    void SetFileName(QString fileName);

    bool GetProductVersion();
    void SetProductVersion(bool bEnable);

private:
    QString mFileName;
    bool mbProductVersion;

    void QueryAndSetVersion(const QString &fileName);
};

#endif // QHUIVERSION_H
