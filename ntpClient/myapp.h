#ifndef MYAPP_H
#define MYAPP_H

#include <QCoreApplication>
#include "./qntp/NtpClient.h"
#include <QTimer>

class MyApp : public QCoreApplication{
    Q_OBJECT
public:
    MyApp(int &argc, char **argv);

private slots:
    void OnNtpReplyReceived(const QHostAddress &address, quint16 port, const NtpReply &reply);

    void OnTimeout();

private:
    NtpClient ntpClient;

    QTimer mTimer;
    bool mbWaitRet;
    int mFailCount;
};

#endif // MYAPP_H
