#include "myapp.h"
#include "cmylogger.h"

int main(int argc, char *argv[])
{
    MyApp a(argc, argv);

    return a.exec();
}
