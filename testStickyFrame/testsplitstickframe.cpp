#include "testsplitstickframe.h"
#include <QtTest>

TestSplitStickFrame::TestSplitStickFrame(QObject *parent) : QObject(parent)
{

}

void TestSplitStickFrame::initTestCase()
{
    Parser.AddCmd("ACMD");
    Parser.AddCmd("BCMD");
    Parser.AddCmd("CCMD");
}

///
/// \brief TestSplitStickFrame::ParseSingleMsg
///单条消息测试
void TestSplitStickFrame::ParseSingleMsg()
{
    QString sCmd = "ACMD";
    QString sData = "data";
    QString sMsg = QString("%1:%2").arg(sCmd).arg(sData);
    QList<SPLIT_FRAME> frames;
    Parser.Parse(sMsg, frames);

    QCOMPARE(frames.size(), 1);
    QCOMPARE(frames.at(0).sCmd, sCmd);
    QCOMPARE(frames.at(0).sData, sData);
}

///
/// \brief TestSplitStickFrame::ParseMultiMsg_data
///多条粘帧信息测试数据生成
void TestSplitStickFrame::ParseMultiMsg_data()
{
    QTest::addColumn<QString>("msg");//待解析消息
    QTest::addColumn<QStringList>("cmdList");//消息包含的命令
    QTest::addColumn<QStringList>("dataList");//消息包含的数据

    QString msg = "ACMD:dataaBCMD:datab";
    QStringList cmdList, dataList;
    cmdList = QStringList()<<"ACMD"<<"BCMD";
    dataList = QStringList()<<"dataa"<<"datab";
    QTest::newRow(qPrintable(msg)) << msg<< cmdList << dataList;

    msg = "ACMD:dataa:dataa2BCMD:databCCMD";
    cmdList = QStringList()<<"ACMD"<<"BCMD"<<"CCMD";
    dataList = QStringList()<<"dataa:dataa2"<<"datab"<<"";
    QTest::newRow(qPrintable(msg)) << msg<< cmdList << dataList;
}

///
/// \brief TestSplitStickFrame::ParseMultiMsg
///多条粘帧信息测试
void TestSplitStickFrame::ParseMultiMsg()
{
    QFETCH(QString, msg);
    QFETCH(QStringList, cmdList);
    QFETCH(QStringList, dataList);

    QList<SPLIT_FRAME> frames;
    Parser.Parse(msg, frames);
    QCOMPARE(frames.size(), cmdList.size());

    for(int i=0; i<frames.size(); i++){
        QCOMPARE(frames.at(i).sCmd, cmdList.at(i));
        QCOMPARE(frames.at(i).sData, dataList.at(i));
    }
}

QTEST_MAIN(TestSplitStickFrame)
