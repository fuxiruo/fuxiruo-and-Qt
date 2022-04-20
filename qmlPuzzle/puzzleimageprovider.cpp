#include "puzzleimageprovider.h"
#include <QDebug>
#include <QtMath>

PuzzleImageProvider::PuzzleImageProvider(): QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap PuzzleImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug()<<__FUNCTION__<<id<<requestedSize;
    if(id.isEmpty()){
        return QPixmap();
    }

    QString totalS = id.section('#', 0, 0);
    QString indexS = id.section('#', 1, 1);
    if(totalS.isEmpty() || indexS.isEmpty()){
        return QPixmap();
    }

    int index = indexS.toInt();
    //qDebug()<<"index:"<<index;
    int total = totalS.toInt();
    //qDebug()<<"total:"<<total;

    if(0==total){
        return QPixmap();
    }

    int q = qSqrt(total);
    int row = index/q;
    int col = index-q*row;
    //qDebug()<<"row,col:"<<row<<col;

    QPixmap p(QUrl(id.section('#', 2)).toLocalFile());
    if(p.isNull()){
        return p;
    }

    if(requestedSize.isValid()){
        p = p.scaled(QSize(q*requestedSize.width(),q*requestedSize.height()), Qt::KeepAspectRatio);
        //qDebug()<<"p"<<p.size();
    }

    int preWidth = p.width()/q;
    int preHeight = p.height()/q;
    QPixmap sub = p.copy(preWidth*col, preHeight*row, preWidth, preHeight);
    //qDebug()<<"sub"<<sub.size();

    if (size){
        *size = QSize(sub.width(), sub.height());
    }

    return sub;
}
