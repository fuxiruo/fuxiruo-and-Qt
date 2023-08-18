#ifndef MODBUSSETTING_H
#define MODBUSSETTING_H

#include <QWidget>

namespace Ui {
class ModbusSetting;
}

class ModbusSetting : public QWidget
{
    Q_OBJECT

public:
    explicit ModbusSetting(QWidget *parent = nullptr);
    ~ModbusSetting();

private:
    Ui::ModbusSetting *ui;
};

#endif // MODBUSSETTING_H
