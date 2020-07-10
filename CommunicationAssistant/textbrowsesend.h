#ifndef TEXTBROWSESEND_H
#define TEXTBROWSESEND_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class TextBrowseSend;
}

class TextBrowseSend : public QWidget
{
    Q_OBJECT

public:
    explicit TextBrowseSend(QWidget *parent = nullptr);
    ~TextBrowseSend();

    QStringList GetHistory();
    void AddHistory(const QStringList &texts);

signals:
    void SigSend(const QByteArray &qbData);

private slots:
    void on_pushButton_clear_clicked();

    void on_pushButton_send_clicked();

    void on_comboBox_encoding_currentIndexChanged(const QString &arg1);

    void on_listView_history_doubleClicked(const QModelIndex &index);

private:
    Ui::TextBrowseSend *ui;

    QString mCodecForName="UTF-8";
    int mnMaxHistory=10;
    QStringList mHistoryList;
    QStringListModel mHistoryModel;

    void Init();
    void AddHistory(const QString &sText);
};

#endif // TEXTBROWSESEND_H
