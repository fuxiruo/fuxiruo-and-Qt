#ifndef SQLDATASMODEL_H
#define SQLDATASMODEL_H

#include <QSqlTableModel>

class SqlDatasModel : public QSqlTableModel
{
    Q_OBJECT
public:
    SqlDatasModel(QObject *parent = 0);

    QString GetTableName() const;
    void SetTableName(const QString &s);
    QStringList GetHeader() const;
    QVariantList GetResult() const;

signals:
    void SigTableNameChanged();

private:
    QString mTableName;
};

#endif // SQLDATASMODEL_H
