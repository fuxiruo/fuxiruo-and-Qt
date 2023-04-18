#ifndef FORMGRID_H
#define FORMGRID_H

#include <QWidget>

namespace Ui {
class FormGrid;
}

class FormGrid : public QWidget
{
    Q_OBJECT

public:
    explicit FormGrid(QWidget *parent = nullptr);
    ~FormGrid();

    void SetRowCol(int row, int col);
    void SetValue(int row, int col, int v);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void OnClicked(bool);

private:
    Ui::FormGrid *ui;

    QWidget *GetWidget(int row, int col);
    void ResizeItems();
};

#endif // FORMGRID_H
