#ifndef TINYHTTPCLIENT_H
#define TINYHTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

class TinyHttpClient : public QObject
{
    Q_OBJECT
public:
    explicit TinyHttpClient(QObject *parent = nullptr);

    void SetTimeout(int interval);
    int GetTimeout();
    void SetUrl(const QString &sUrl);

    void StartRequest(const QString &sBody);

signals:
    void SigReply(QNetworkReply::NetworkError, const QString &errorString, const QString &reply);

public slots:

private slots:
    void OnWaitTimeout();

    void OnReplyFinished();

private:
    QTimer mTimer;
    QUrl url;
    QNetworkRequest request;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

    void AbortRequest();
};

#endif // TINYHTTPCLIENT_H
