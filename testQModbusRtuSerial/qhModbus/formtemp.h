#ifndef FORMTEMP_H
#define FORMTEMP_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class FormTemp;
}

class FormTemp : public QWidget
{
    Q_OBJECT

public:
    explicit FormTemp(QWidget *parent = nullptr);
    ~FormTemp();

    void SetTitle(const QString &s);

    void SetAddr(int addr);
    int GetAddr();

    void UpdateTemp(float_t t);
    float_t GetTemp();

    void UpdateSetTemp(float_t t);

    void ReStartTimer();

signals:
    void SigSetTemp(int addr, float_t t);

private slots:
    void OnTimeout();
    void OnSetTempTimeout();

    void on_pushButtonSetTemp_clicked();

private:
    Ui::FormTemp *ui;
    QTimer mTimer;
    QTimer mSetTimer;

    int mAddr;
    float_t mSetTemp;
};

#endif // FORMTEMP_H
