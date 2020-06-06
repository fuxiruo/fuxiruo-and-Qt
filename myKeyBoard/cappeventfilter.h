#ifndef CAPPEVENTFILTER_H
#define CAPPEVENTFILTER_H

#include <QObject>

class myKeyBoard;
class CAppEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CAppEventFilter(QObject *parent = nullptr);

    void SetKeyBoard(myKeyBoard *pKeyBoard);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:

private:
    myKeyBoard *mpKeyBoard;
};

#endif // CAPPEVENTFILTER_H
