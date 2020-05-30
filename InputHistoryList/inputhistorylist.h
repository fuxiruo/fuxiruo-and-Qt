#ifndef INPUTHISTORYLIST_H
#define INPUTHISTORYLIST_H

#include <QListWidget>

class InputHistoryList : public QListWidget
{
    Q_OBJECT
public:
    explicit InputHistoryList(QWidget *parent = nullptr);//parent一般设置为pShowUnderWidget的parentWidget

    //设置显示在哪个控件下方,一般是输入性控件，如QSpinBox、QLineEdit，bAutoShowHide设置是否自动判断显示和隐藏
    void SetShowUnderWidget(QWidget *pShowUnderWidget, bool bAutoShowHide=true);

    void SetHistory(const QStringList &labels);//设置历史输入项

    void ReShow();

signals:

public slots:

private slots:
    void OnItemClicked(QListWidgetItem *item);

private:
    QWidget *mShowUnderWidget;

    bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // INPUTHISTORYLIST_H
