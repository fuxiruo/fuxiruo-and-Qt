#include <QDateTime>
#include <QDebug>
#include "cmylogger.h"

CMyLogger::CMyLogger(QObject *parent) : QObject(parent)
{

}

CMyLogger::~CMyLogger()
{
    if(mLogFile.isOpen()){
        mLogStream.flush();
        mLogFile.close();
    }
}

void CMyLogger::CPrintfToLog(LOGLEVEL eLogLevel, const char *pFun, char *fmt, ...)
{
    char Buff[1024*1024];
    va_list ReportStatusList;
    va_start(ReportStatusList, fmt);
    vsprintf(Buff, fmt, ReportStatusList);
    va_end(ReportStatusList);

    QDateTime nowDateTime = QDateTime::currentDateTime();
    QString sTime = nowDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString sBuff = QString(Buff);
    QString sFun = QString(pFun);
    QString sMsg = QString("[%1]").arg(sTime);

    switch(eLogLevel){
    case eLogLevel_DBG:{
        sMsg += "[DBG]";
        //logger()->debug(sMsg);
    }break;

    case eLogLevel_INF:{
        sMsg += "[INF]";
        //logger()->info(sMsg);
    }break;

    case eLogLevel_WRN:{
        sMsg += "[WRN]";
        //logger()->warn(sMsg);
    }break;

    case eLogLevel_ERR:{
        sMsg += "[ERR]";
        //logger()->error(sMsg);
    }break;

    default:
        break;
    }
    sMsg += QString("[%1]->").arg(sFun);
    sMsg += sBuff;

    qDebug()<<sMsg;
    LogToFile(sMsg, eLogLevel);
}

bool CMyLogger::SetLogFile(QString sFile)
{
    mLogFile.setFileName(sFile);
    if(mLogFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug()<<"open logger file success:"<<sFile;
        mLogStream.setDevice(&mLogFile);
        return true;
    }else {
        qDebug()<<"open logger file faile:"<<sFile<<mLogFile.errorString();
        return false;
    }
}

void CMyLogger::LogToFile(QString sLog, LOGLEVEL eLogLevel)
{
    if(mLogFile.isOpen()){
        mLogStream<<sLog<<endl;
        if(eLogLevel == eLogLevel_DBG){
            mLogStream.flush();
        }
    }
}
