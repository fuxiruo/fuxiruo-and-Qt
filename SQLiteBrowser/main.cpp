#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QSqlError>
#include "sqldatasmodel.h"
#include "sqlmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<SqlModel>("ko.sql.model", 1, 0, "SqlModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
