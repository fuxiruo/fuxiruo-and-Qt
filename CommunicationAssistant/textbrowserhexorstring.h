#ifndef TEXTBROWSERHEXORSTRING_H
#define TEXTBROWSERHEXORSTRING_H

#include <QWidget>

namespace Ui {
class TextBrowserHexOrString;
}

class TextBrowserHexOrString : public QWidget
{
    Q_OBJECT

public:
    explicit TextBrowserHexOrString(QWidget *parent = nullptr);
    ~TextBrowserHexOrString();

    void Append(const QByteArray &data);
    void Append(const QString &IP, int Port, const QByteArray &data);
    void SetCodecForName(const QString &codecForName);

private slots:
    void on_pushButton_clear_clicked();

    void on_comboBox_encoding_currentIndexChanged(const QString &arg1);

private:
    Ui::TextBrowserHexOrString *ui;
    QString mCodecForName="UTF-8";

    QString ConvText(const QByteArray &data);
};

#endif // TEXTBROWSERHEXORSTRING_H
