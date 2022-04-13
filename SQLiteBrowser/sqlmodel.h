#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QObject>
#include <QVariant>
#include <QSqlDatabase>

class SqlTablesModel;
class SqlDatasModel;
class SqlModel : public QObject
{
    Q_OBJECT
public:
    explicit SqlModel(QObject *parent = nullptr);

    Q_INVOKABLE void qmlOpenDatabase(const QUrl &url);
    Q_INVOKABLE void qmlSetTableName(const QString &s);
    Q_INVOKABLE QStringList qmlGetHeader() const;
    Q_INVOKABLE QVariantList qmlGetResult() const;
    Q_INVOKABLE QStringList qmlGetTables() const;
signals:

public slots:

private:
    QSqlDatabase database;
    SqlDatasModel *mDatas;

    void OpenDataBase(const QString &fileName);
};

#endif // SQLMODEL_H
