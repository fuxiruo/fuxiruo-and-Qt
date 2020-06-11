#ifndef MYSCROLLTEXT_H
#define MYSCROLLTEXT_H

#include <QWidget>
#include <QTimer>

class MyScrollText : public QWidget
{
    Q_OBJECT
public:
    explicit MyScrollText(QWidget *parent = nullptr);

    QSize sizeHint() const;

    QString text() const;
    void SetText(QString text);
    void SetInterval(int v);//滚动速度
    void SetScrollSteps(int v);//滚动步数

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void OnTimeout();

private:
    QString msText;
    int mnHeadOffset;
    int mnTailOffset;
    int mnScrollSteps = 1;
    QTimer mScrollTimer;

    void InitOffset();
};

#endif // MYSCROLLTEXT_H
