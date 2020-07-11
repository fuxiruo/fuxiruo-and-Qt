#include "myapp.h"
#include "cmylogger.h"

CMyLogger logger;

int main(int argc, char *argv[])
{
    logger.SetLogFile("ntpClient.log");
    MyApp a(argc, argv);

    INF("+++++++++++++++++++++++++++++++++");
    INF("build on %s %s",__DATE__, __TIME__);

    return a.exec();
}
