#ifndef TESTPLUIN_H
#define TESTPLUIN_H

#include <QWidget>

class TestPluin : public QWidget
{
    Q_OBJECT

    typedef QMap<QString, int> MapStringInt;
    Q_DECLARE_METATYPE(MapStringInt)


    Q_PROPERTY(MapStringInt name READ name WRITE setName)

public:
    TestPluin(QWidget *parent = 0);

    MapStringInt name();
    void setName();
};

#endif
