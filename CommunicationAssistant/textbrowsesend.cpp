#include "textbrowsesend.h"
#include "ui_textbrowsesend.h"
#include <QTextCodec>
#include <QDateTime>
#include <QtEndian>
#include <QSettings>
#include <QTimer>
#include <commonfunc.h>

TextBrowseSend::TextBrowseSend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextBrowseSend)
{
    ui->setupUi(this);

    Init();
    QTimer::singleShot(0, this, SLOT(OnInitAfterUI()));
}

TextBrowseSend::~TextBrowseSend()
{
    SaveSendHistory();
    delete ui;
}

QStringList TextBrowseSend::GetHistory()
{
    return mHistoryModel.stringList();
}

void TextBrowseSend::SetHistory(const QStringList &texts)
{
    mHistoryModel.setStringList(texts);
}

QString TextBrowseSend::GetAutoHead()
{
    return ui->lineEdit_head->text();
}

void TextBrowseSend::SetAutoHead(const QString &sText)
{
    ui->lineEdit_head->setText(sText);
}

QString TextBrowseSend::GetAutoTail()
{
    return ui->lineEdit_tail->text();
}

void TextBrowseSend::SetAutoTail(const QString &sText)
{
    ui->lineEdit_tail->setText(sText);
}

bool TextBrowseSend::event(QEvent *event)
{
    if(event->type() == QEvent::DynamicPropertyChange){
        QDynamicPropertyChangeEvent *qde = static_cast<QDynamicPropertyChangeEvent *>(event);
        if("group" == qde->propertyName()){
            ui->userdefined->SetGroup(property(qde->propertyName()).toString());
        }
    }

    return QWidget::event(event);
}

void TextBrowseSend::OnInitAfterUI()
{
    LoadSendHistroy();
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

QString TextBrowseSend::GetGroup()
{
    if(property("group").isValid()){
        return property("group").toString();
    }else{
        return "";
    }
}

void TextBrowseSend::LoadSendHistroy()
{
    QSettings setting(CommonFunc::GetAppPath()+"/history.ini", QSettings::IniFormat);
    setting.beginGroup(GetGroup());
    QStringList historyList;
    foreach (QString sKey, setting.allKeys()) {
        QString sValue = setting.value(sKey).toString();
        if(!sValue.isEmpty()){
            historyList.append(sValue);
        }
    }
    mHistoryModel.setStringList(historyList);
    setting.endGroup();
}

void TextBrowseSend::SaveSendHistory()
{
    QSettings setting(CommonFunc::GetAppPath()+"/history.ini", QSettings::IniFormat);
    setting.beginGroup(GetGroup());
    for(int i=0; i<mHistoryModel.stringList().size(); i++){
        setting.setValue(QString::number(i), mHistoryModel.stringList().at(i));
    }
    setting.endGroup();
}

void TextBrowseSend::AddHistory(const QString &sText)
{
    QString sNewText = QString("[%1]%2")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
            .arg(sText);

    mHistoryModel.insertRow(mHistoryModel.rowCount());
    mHistoryModel.setData(mHistoryModel.index(mHistoryModel.rowCount()-1), sNewText);

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

    QByteArray qbSend;
    qbSend.clear();
    if(ui->groupBox_add_headSizeTail->isChecked()){
        char chNum[4];
        qToBigEndian(qbData.size(), chNum);

        if(!ui->lineEdit_head->text().isEmpty()){
            if(ui->lineEdit_head->text().startsWith("0x")){
                QStringList hexList = ui->lineEdit_head->text().split(QRegExp("\\s+"));
                bool ok;
                foreach (QString hex, hexList) {
                    qbSend.append(hex.toInt(&ok, 16));
                }
            }else{
                qbSend.append(ui->lineEdit_head->text());
            }
        }

        qbSend.append(chNum, 4);
    }

    qbSend.append(qbData);

    if(ui->groupBox_add_headSizeTail->isChecked()){
        if(!ui->lineEdit_tail->text().isEmpty()){
            if(ui->lineEdit_tail->text().startsWith("0x")){
                QStringList hexList = ui->lineEdit_tail->text().split(QRegExp("\\s+"));
                bool ok;
                foreach (QString hex, hexList) {
                    qbSend.append(hex.toInt(&ok, 16));
                }
            }else{
                qbSend.append(ui->lineEdit_tail->text());
            }
        }
    }

    emit SigSend(qbSend);
}

void TextBrowseSend::on_listView_history_doubleClicked(const QModelIndex &index)
{
    QString sHistory = index.data().toString();
    sHistory.remove(0, strlen("[yyyy-MM-dd HH:mm:ss.zzz]"));
    ui->textEdit->setText(sHistory);
}
