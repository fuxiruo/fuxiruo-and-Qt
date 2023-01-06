#include "mainwindow.h"
#include <QApplication>

#define QT_MESSAGE_PATTERN "[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}]%{message}"

int main(int argc, char *argv[])
{
    qSetMessagePattern(QT_MESSAGE_PATTERN);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
