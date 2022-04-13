#include "sqldatasmodel.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

SqlDatasModel::SqlDatasModel(QObject *parent) :
    QSqlTableModel(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    SetTableName("");
}

QString SqlDatasModel::GetTableName() const
{
    return mTableName;
}

void SqlDatasModel::SetTableName(const QString &s)
{
    if(s == mTableName){
        return;
    }
    mTableName = s;

    if(mTableName.isEmpty()){
        return;
    }

    setTable(mTableName);
    if(!select()){
        qWarning("select table failed: %s", qPrintable(lastError().text()));
    }
    qDebug()<<"SetTableName select succ";
    emit SigTableNameChanged();
}

QStringList SqlDatasModel::GetHeader() const
{
    QStringList headers;
    const QSqlRecord sqlRecord = record();
    for(int i=0; i<sqlRecord.count(); i++){
        headers.append(sqlRecord.fieldName(i));
    }
    return headers;
}

QVariantList SqlDatasModel::GetResult() const
{
    int r = 0;
    QVariantList val;
    QSqlRecord sqlRecord;
    while(r < rowCount()){
        sqlRecord = record(r);
        //qDebug()<<r<<sqlRecord<<sqlRecord.isEmpty();
        if(sqlRecord.isEmpty()){
            break;
        }

        QVariantMap dataMap;
        for(int i=0; i<sqlRecord.count(); i++){
            dataMap.insert(sqlRecord.fieldName(i), sqlRecord.value(i));
        }
        val.append(QVariant::fromValue(dataMap));

        r++;
    }

    return val;
}
