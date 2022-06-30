#include "pngremovetask.h"
#include <QDebug>
#include <QMutexLocker>
#include <QImage>

PngRemoveTask::PngRemoveTask()
{

}

void PngRemoveTask::SetArg(const QString &sPng)
{
    msPng = sPng;
}

void PngRemoveTask::SetMutex(QMutex *gMutex, QList<PngRemoveResult> *gResult)
{
    mMutex = gMutex;
    mResult = gResult;
}

void PngRemoveTask::run()
{
    //qDebug()<<msPng<<"start";
    PngRemoveResult result;
    result.file = msPng;
    result.result = QString();

    QImage image( msPng, "PNG" );

    if ( image.isNull())
    {
        result.result = "openSourceError";
    }

    QImage image2( image.bits(), image.width(), image.height(), QImage::Format_ARGB32 );
    if ( image2.isNull() || !image2.save( msPng ) )
    {
        result.result = "saveTargetError";
    }

    QMutexLocker locker(mMutex);
    mResult->append(result);
    //qDebug()<<msPng<<"end";
}
