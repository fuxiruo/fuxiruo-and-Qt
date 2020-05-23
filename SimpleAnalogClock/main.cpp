#include "mainwindow.h"
#include <QApplication>
#include "simpleanalogclock.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleAnalogClock w;
    w.show();

    return a.exec();
}
