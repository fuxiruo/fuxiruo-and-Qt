#ifndef IMPORTEXPORTPARAMETER_H
#define IMPORTEXPORTPARAMETER_H

#include <QObject>

struct Parameter
{
    QString str;
    quint32 uNum;
};

class ImportExportParameter : public QObject
{
    Q_OBJECT
public:
    enum ErrCode {
        OK = 0,
        ErrHead,//文件头错误
        ErrVersion,//文件版本不兼容
        ErrData,//数据异常，如参数个数不对
        ErrVerify,//校验失败
        Unknown,
    };

    explicit ImportExportParameter(QObject *parent = nullptr);

    static bool Export(const QString &sFile, const QVector<Parameter> &v);
    static ErrCode Import(const QString &sFile, QVector<Parameter> &v);
signals:

public slots:
};

#endif // IMPORTEXPORTPARAMETER_H
