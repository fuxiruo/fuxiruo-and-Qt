#include "myapp.h"
#include "cmylogger.h"

CMyLogger logger;

int main(int argc, char *argv[])
{
    MyApp a(argc, argv);

    logger.SetLogFile("ntpClient.log");
    INF("+++++++++++++++++++++++++++++++++");
    INF("build on %s %s",__DATE__, __TIME__);

    return a.exec();
}
