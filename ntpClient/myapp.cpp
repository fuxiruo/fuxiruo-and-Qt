#include "myapp.h"
#include <windows.h>
#include "cmylogger.h"
#include "./qntp/NtpReply.h"

CMyLogger logger;

MyApp::MyApp(int &argc, char **argv):QCoreApplication(argc, argv)
{
    logger.SetLogFile(QCoreApplication::applicationDirPath() + "/ntpClient.log");
    INF("+++++++++++++++++++++++++++++++++");
    INF("build on %s %s",__DATE__, __TIME__);

    mbWaitRet = false;
    mFailCount = 0;

    QDate qBuildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " " ), "MMM dd yyyy");
    QDate qNowDate = QDate::currentDate();

    if(qBuildDate > qNowDate){
       INF("now date %s, build date %s!(时间异常)",
               qNowDate.toString("MMM dd yyyy").toUtf8().constData(),
               qBuildDate.toString("MMM dd yyyy").toUtf8().constData());
       INF("自动修正时间...");
       QDateTime dateTime = QDateTime(qBuildDate);
       SetSysDateTime(dateTime);
    }

    ntpClient.init("cn.pool.ntp.org");
    connect(&ntpClient, SIGNAL(replyReceived(QHostAddress,quint16,NtpReply)), this, SLOT(OnNtpReplyReceived(QHostAddress,quint16,NtpReply)));

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
    mTimer.start(3000);
}

void MyApp::OnNtpReplyReceived(const QHostAddress &/*address*/, quint16 /*port*/, const NtpReply &reply)
{
    mFailCount = 0;
    mbWaitRet = false;
    if(reply.localClockOffset() > -1000 && reply.localClockOffset() < 1000){
        INF("time sync succ");
        this->exit();
    }else{
        QDateTime syncDateTime = QDateTime::currentDateTime().addMSecs(reply.localClockOffset());
        INF("sync time:%s", qUtf8Printable(syncDateTime.toString("yyyy-MM-dd HH:mm:ss")));

        SetSysDateTime(syncDateTime);
    }
}

void MyApp::OnTimeout()
{
    if(mbWaitRet){
        mFailCount++;
        if(mFailCount >= 10){
            ERR("sync fail");
            this->exit();
        }
    }

    if(QAbstractSocket::ConnectedState != ntpClient.state()){
        WRN("connect fail");
        return;
    }else{
        mbWaitRet = true;
        ntpClient.sendRequest();
    }
}

void MyApp::SetSysDateTime(QDateTime dateTime)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    st.wYear = dateTime.date().year();
    st.wMonth = dateTime.date().month();
    st.wDay = dateTime.date().day();
    st.wHour = dateTime.time().hour();
    st.wMinute = dateTime.time().minute();
    st.wSecond = dateTime.time().second();
    SetLocalTime(&st);
}
