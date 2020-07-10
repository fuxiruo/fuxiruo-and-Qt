#include "textbrowserhexorstring.h"
#include "ui_textbrowserhexorstring.h"
#include <QTextCodec>
#include <QDateTime>

TextBrowserHexOrString::TextBrowserHexOrString(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextBrowserHexOrString)
{
    ui->setupUi(this);
}

TextBrowserHexOrString::~TextBrowserHexOrString()
{
    delete ui;
}

void TextBrowserHexOrString::Append(const QByteArray &data)
{
    QString sNewText;
    if(ui->checkBox_timestamp->isChecked()){
        sNewText += QString("[%1]").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"));
    }
    sNewText += ConvText(data);
    ui->textEdit->append(sNewText);
}

void TextBrowserHexOrString::Append(const QString &IP, int Port, const QByteArray &data)
{
    QString sNewText;
    if(ui->checkBox_timestamp->isChecked()){
        sNewText += QString("[%1]%2:%3->")
                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                    .arg(IP).arg(Port);
    }
    sNewText += ConvText(data);
    ui->textEdit->append(sNewText);
}

void TextBrowserHexOrString::SetCodecForName(const QString &codecForName)
{
    mCodecForName = codecForName;
}

void TextBrowserHexOrString::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

QString TextBrowserHexOrString::ConvText(const QByteArray &data)
{
    QString text;
    if(ui->radioButton_hex->isChecked()){
        text = data.toHex(' ');
    }else if(ui->radioButton_string->isChecked()){
        QTextCodec *pGBKEncoder=QTextCodec::codecForName(mCodecForName.toLocal8Bit());
        text = pGBKEncoder->toUnicode(data);
    }else{
        QTextCodec *pGBKEncoder=QTextCodec::codecForName(mCodecForName.toLocal8Bit());
        text = QString("%1(%2)")
                .arg(pGBKEncoder->toUnicode(data))
                .arg(QString(data.toHex(' ')));
    }

    return text;
}

void TextBrowserHexOrString::on_comboBox_encoding_currentIndexChanged(const QString &arg1)
{
    SetCodecForName(arg1);
}
