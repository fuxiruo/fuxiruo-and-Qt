#include "textbrowsesend.h"
#include "ui_textbrowsesend.h"
#include <QTextCodec>
#include <QDateTime>

TextBrowseSend::TextBrowseSend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextBrowseSend)
{
    ui->setupUi(this);

    Init();
}

TextBrowseSend::~TextBrowseSend()
{
    delete ui;
}

QStringList TextBrowseSend::GetHistory()
{
    return mHistoryList;
}

void TextBrowseSend::AddHistory(const QStringList &texts)
{
    mHistoryList = texts;
    mHistoryModel.setStringList(mHistoryList);
}

void TextBrowseSend::OnUserdefinedSend(QString sMsg)
{
    AddHistory(sMsg);

    Send(sMsg);
}

void TextBrowseSend::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

void TextBrowseSend::on_pushButton_send_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty()){
        return;
    }

    AddHistory(ui->textEdit->toPlainText());

    Send(ui->textEdit->toPlainText());
}

void TextBrowseSend::on_comboBox_encoding_currentIndexChanged(const QString &arg1)
{
    mCodecForName = arg1;
}

void TextBrowseSend::Init()
{
    ui->listView_history->setModel(&mHistoryModel);

    connect(ui->userdefined, SIGNAL(SigSend(QString)), this, SLOT(OnUserdefinedSend(QString)));
}

void TextBrowseSend::AddHistory(const QString &sText)
{
    QString sNewText = QString("[%1]%2")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(sText);

    mHistoryList.append(sNewText);
    if(mHistoryList.size() > mnMaxHistory){
        mHistoryList.pop_front();
    }
    mHistoryModel.setStringList(mHistoryList);

    ui->listView_history->scrollToBottom();
}

void TextBrowseSend::Send(QString sMsg)
{
    QByteArray qbData;
    if(ui->radioButton_string->isChecked()){
        QTextCodec *pGBKEncoder=QTextCodec::codecForName(mCodecForName.toLocal8Bit());
        qbData = pGBKEncoder->fromUnicode(sMsg);
    }else{
        qbData = QByteArray::fromHex(sMsg.toLocal8Bit());
    }

    emit SigSend(qbData);
}

void TextBrowseSend::on_listView_history_doubleClicked(const QModelIndex &index)
{
    QString sHistory = index.data().toString();
    sHistory.remove(0, strlen("[yyyy-MM-dd HH:mm:ss.zzz]"));
    ui->textEdit->setText(sHistory);
}
