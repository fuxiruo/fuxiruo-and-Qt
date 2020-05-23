#ifndef SIMPLEANALOGCLOCK_H
#define SIMPLEANALOGCLOCK_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>

class SimpleAnalogClock : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleAnalogClock(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private Q_SLOTS:
    void OnTimeout();

private:
    QTimer mTimer;
    QDateTime mLastDateTime;
};

#endif // SIMPLEANALOGCLOCK_H
