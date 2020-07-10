#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>

namespace Ui {
class SerialWindow;
}

class SerialWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SerialWindow(QWidget *parent = nullptr);
    ~SerialWindow();

private:
    Ui::SerialWindow *ui;
};

#endif // SERIALWINDOW_H
