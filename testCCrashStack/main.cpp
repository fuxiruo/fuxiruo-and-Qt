#include "mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include "CCrashStack/ccrashstack.h"

#ifdef Q_OS_WIN
long __stdcall   callback(_EXCEPTION_POINTERS*   excp)
{
    CCrashStack crashStack(excp);
    QString sCrashInfo = crashStack.GetExceptionInfo();
    QString file_path = QApplication::applicationDirPath();

    QDateTime dateTime = QDateTime::currentDateTime();
    QString baseFileName = "/crash"+ dateTime.toString("yyyyMMddhhmmss") + ".log";
    QString sFileName = file_path + baseFileName;

    QFile file(sFileName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        file.write(sCrashInfo.toUtf8());
        file.close();
    }

    return   EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char *argv[])
{    // 设置异常处理函数
#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter(callback);
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
