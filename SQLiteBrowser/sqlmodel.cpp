#include "sqlmodel.h"
#include <QDebug>
#include <QSqlError>
#include <QGuiApplication>
#include <QUrl>
#include "sqldatasmodel.h"

SqlModel::SqlModel(QObject *parent) : QObject(parent)
{
    qDebug()<<"SqlModel";
    database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    mDatas = new SqlDatasModel(this);
}

void SqlModel::qmlOpenDatabase(const QUrl &url)
{
    qDebug()<<__FUNCTION__<<url;
    OpenDataBase(url.toLocalFile());
}

void SqlModel::qmlSetTableName(const QString &s)
{
    qDebug()<<"qmlSetTableName:"<<s;
    mDatas->SetTableName(s);
}

QStringList SqlModel::qmlGetHeader() const
{
    if(!database.isOpen()){
        return QStringList();
    }
    return mDatas->GetHeader();
}

QVariantList SqlModel::qmlGetResult() const
{
    return mDatas->GetResult();
}

QStringList SqlModel::qmlGetTables() const
{
    if(!database.isOpen()){
        return QStringList();
    }

    QStringList qsl = database.tables();
    qsl.sort();
    qDebug()<<"qmlGetTables:"<<qsl;
    return qsl;
}

void SqlModel::OpenDataBase(const QString &fileName)
{
    if(database.isOpen()){
        database.close();
    }

    mDatas->SetTableName("");
    // When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
    }
    qDebug()<<database;
}
