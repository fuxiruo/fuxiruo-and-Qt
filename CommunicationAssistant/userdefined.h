#ifndef USERDEFINED_H
#define USERDEFINED_H

#include <QWidget>
#include <QSet>

namespace Ui {
class UserDefined;
}

class UserDefined : public QWidget
{
    Q_OBJECT

public:
    explicit UserDefined(QWidget *parent = nullptr);
    ~UserDefined();

protected:
    void showEvent(QShowEvent *event);

Q_SIGNALS:
    void SigSend(QString);

private Q_SLOTS:
    void OnBtnClick();
    void OnTextEdited(const QString &);
    void OnLabelLinkActivated(const QString &link);

private:
    Ui::UserDefined *ui;

    enum{E_COL_OF_INDEX=0, E_COL_OF_LINEEDIT, E_COL_OF_SEND_BTN}E_COL_INDEX;
    QSet<int> mChangeColList;
    int mRowCount;

    void Add();
    void Init();
    void Save();
    void Load();
};

#endif // USERDEFINED_H
