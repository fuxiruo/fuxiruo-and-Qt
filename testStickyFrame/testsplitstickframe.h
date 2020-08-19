#ifndef TESTSPLITSTICKFRAME_H
#define TESTSPLITSTICKFRAME_H

#include <QObject>
#include "splitstickframe.h"

class TestSplitStickFrame : public QObject
{
    Q_OBJECT
public:
    explicit TestSplitStickFrame(QObject *parent = nullptr);

signals:

public slots:

private slots:
    void initTestCase();

    void ParseSingleMsg();

    void ParseMultiMsg_data();
    void ParseMultiMsg();

private:
    SplitStickFrame Parser;
};

#endif // TESTSPLITSTICKFRAME_H
