#include "tinyhttpclient.h"
#include <QDebug>

TinyHttpClient::TinyHttpClient(QObject *parent) : QObject(parent)
{
    reply = nullptr;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    SetTimeout(10*1000);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(OnWaitTimeout()));
    mTimer.setSingleShot(true);
}

void TinyHttpClient::SetTimeout(int interval)
{
    mTimer.setInterval(interval);
}

int TinyHttpClient::GetTimeout()
{
    return mTimer.interval();
}

void TinyHttpClient::SetUrl(const QString &sUrl)
{
    url.setUrl(sUrl);

    request.setUrl(url);
}

void TinyHttpClient::StartRequest(const QString &sBody)
{
    qDebug()<<__FUNCTION__;
    if(reply){
        qWarning()<<"Last requst not finished, abort";
        AbortRequest();
    }

    reply = qnam.sendCustomRequest(request, "GET", sBody.toUtf8());
    connect(reply, SIGNAL(finished()), this, SLOT(OnReplyFinished()));
    mTimer.start();
}

void TinyHttpClient::OnWaitTimeout()
{
    qWarning()<<__FUNCTION__;
    emit SigReply(QNetworkReply::TimeoutError, "超时", "");

    AbortRequest();
}

void TinyHttpClient::OnReplyFinished()
{
    mTimer.stop();

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<__FUNCTION__<<status_code;

    if(reply->error() == QNetworkReply::NoError){
        qDebug()<<reply->rawHeaderList();
        QString msg = QString::fromUtf8(reply->readAll());
        qDebug()<<msg;
        emit SigReply(reply->error(), reply->errorString(), msg);
    }else{
        qWarning()<<status_code<<reply->errorString();
        emit SigReply(reply->error(), reply->errorString(), "");
    }

    reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater
    reply = nullptr;
}

void TinyHttpClient::AbortRequest()
{
    mTimer.stop();
    reply->disconnect();
    reply->abort();
    reply->deleteLater();
    reply = nullptr;
}
