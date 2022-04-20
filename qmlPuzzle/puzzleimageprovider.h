#ifndef PUZZLEIMAGEPROVIDER_H
#define PUZZLEIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>

class PuzzleImageProvider : public QQuickImageProvider
{
public:
    PuzzleImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) Q_DECL_OVERRIDE;
};

#endif // PUZZLEIMAGEPROVIDER_H
