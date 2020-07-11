#ifndef CMYLOGGER_H
#define CMYLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

typedef enum enumAKELogLevel{
    eLogLevel_DBG = 0,
    eLogLevel_INF,
    eLogLevel_WRN,
    eLogLevel_ERR,
    eLogLevel_FTL
}LOGLEVEL;

class CMyLogger : public QObject
{
    Q_OBJECT
public:
    explicit CMyLogger(QObject *parent = nullptr);
    ~CMyLogger();

    void CPrintfToLog(LOGLEVEL eLogLevel, const char *pFun, char *fmt, ...);

    bool SetLogFile(QString sFile);

private:
    QFile mLogFile;
    QTextStream mLogStream;
    void LogToFile(QString sLog, LOGLEVEL eLogLevel);

signals:

public slots:
};

extern CMyLogger logger;
#define DBG(X,...)   logger.CPrintfToLog(eLogLevel_DBG, __FUNCTION__, X, ##__VA_ARGS__)
#define INF(X,...)   logger.CPrintfToLog(eLogLevel_INF, __FUNCTION__, X, ##__VA_ARGS__)
#define WRN(X,...)   logger.CPrintfToLog(eLogLevel_WRN, __FUNCTION__, X, ##__VA_ARGS__)
#define ERR(X,...)   logger.CPrintfToLog(eLogLevel_ERR, __FUNCTION__, X, ##__VA_ARGS__)

#endif // CMYLOGGER_H
