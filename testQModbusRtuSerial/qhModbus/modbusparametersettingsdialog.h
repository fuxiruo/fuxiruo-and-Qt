#ifndef MODBUSPARAMETERSETTINGSDIALOG_H
#define MODBUSPARAMETERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ModbusParameterSettingsDialog;
}

class ModbusParameterSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModbusParameterSettingsDialog(QWidget *parent = nullptr);
    ~ModbusParameterSettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ModbusParameterSettingsDialog *ui;
};

#endif // MODBUSPARAMETERSETTINGSDIALOG_H
