#include "commonfunc.h"
#include <QApplication>

CommonFunc::CommonFunc()
{

}

QString CommonFunc::GetAppPath()
{
    static QString sApp = "";
    if(sApp == ""){
        sApp = QApplication::applicationDirPath();
    }

    return sApp;
}
