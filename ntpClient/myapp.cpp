#include "myapp.h"
#include <QDateTime>
#include <windows.h>
#include "cmylogger.h"
#include "./qntp/NtpReply.h"

MyApp::MyApp(int &argc, char **argv):QCoreApplication(argc, argv)
{
    mbWaitRet = false;
    mFailCount = 0;

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

        SYSTEMTIME st;
        GetLocalTime(&st);
        st.wYear = syncDateTime.date().year();
        st.wMonth = syncDateTime.date().month();
        st.wDay = syncDateTime.date().day();
        st.wHour = syncDateTime.time().hour();
        st.wMinute = syncDateTime.time().minute();
        st.wSecond = syncDateTime.time().second();
        SetLocalTime(&st);
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
