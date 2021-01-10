#ifndef QHSIMPLESELECT_H
#define QHSIMPLESELECT_H

#include <QWidget>
#include <QButtonGroup>
#include <QtWidgets/QBoxLayout>
#include <QRadioButton>
#include <QList>

namespace Ui {
class SimpleSelect;
}

#ifdef TEST_WITH_SRC
class SimpleSelect : public QWidget
#else
class QDESIGNER_WIDGET_EXPORT SimpleSelect : public QWidget
#endif
{
    Q_OBJECT

    Q_PROPERTY(Direction direction READ GetDirection WRITE SetDirection)

    //文本和数值键值对应关系，每行对应一个键值对，如0:停用; 1:启用,数值不能重复，否则提示未定义
    Q_PROPERTY(QStringList valueToText READ GetValueToText WRITE SetValueToText)

public:
    enum Direction { LeftToRight = QBoxLayout::LeftToRight, TopToBottom = QBoxLayout::TopToBottom};
    Q_ENUM(Direction)

    explicit SimpleSelect(QWidget *parent = nullptr);
    ~SimpleSelect();

    Direction GetDirection();
    void SetDirection(Direction d);

    QStringList GetValueToText();
    void SetValueToText(const QStringList &v);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void OnButtonToggled(int id);

signals:
    void SigSelectChange(int value, QString sText);        //选择项改变后发出信号，value为当前选择项定义的数值,sText是其文本

private:
    Ui::SimpleSelect *ui;

    QList<QRadioButton *> mSelectList;
    QStringList msTextList;
    QStringList msValueList;
    QButtonGroup mButtonGroup;
    QStringList mValueToText;

    void AddSelect();
    void RemoveSelect();
    void SetCheckNone();
    void AutoAddTextAndValue(QString text, QString value);
    void AutoSetSelect();
};

#endif // QHSIMPLESELECT_H
