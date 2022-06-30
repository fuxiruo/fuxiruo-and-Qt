#ifndef PNGREMOVETASK_H
#define PNGREMOVETASK_H

#include <QRunnable>
#include <QString>
#include <QMutex>

struct PngRemoveResult {
      QString file;
      QString result;
};

class PngRemoveTask : public QRunnable
{
public:
    PngRemoveTask();
    void SetArg(const QString &sPng);
    void SetMutex(QMutex *gMutex, QList<PngRemoveResult> *gResult);

    void run();

private:
    QString msPng;

    QMutex *mMutex;
    QList<PngRemoveResult> *mResult;
};

#endif // PNGREMOVETASK_H
