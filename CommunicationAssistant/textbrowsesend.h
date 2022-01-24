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
    void SetHistory(const QStringList &texts);

    QString GetAutoHead();
    void SetAutoHead(const QString &sText);
    QString GetAutoTail();
    void SetAutoTail(const QString &sText);

protected:
    // Event handlers
    bool event(QEvent *event) Q_DECL_OVERRIDE;

signals:
    void SigSend(const QByteArray &qbData);

private slots:
    void OnInitAfterUI();

    void OnUserdefinedSend(QString sMsg);

    void on_pushButton_clear_clicked();

    void on_pushButton_send_clicked();

    void on_comboBox_encoding_currentIndexChanged(const QString &arg1);

    void on_listView_history_doubleClicked(const QModelIndex &index);

private:
    Ui::TextBrowseSend *ui;

    QString mCodecForName="UTF-8";
    int mnMaxHistory=10;
    QStringListModel mHistoryModel;

    void Init();
    QString GetGroup();
    void LoadSendHistroy();
    void SaveSendHistory();
    void AddHistory(const QString &sText);
    void Send(QString sMsg);
};

#endif // TEXTBROWSESEND_H
