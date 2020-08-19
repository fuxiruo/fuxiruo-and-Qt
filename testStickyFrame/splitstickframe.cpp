#include "splitstickframe.h"
#include <QDebug>

SplitStickFrame::SplitStickFrame()
{

}

void SplitStickFrame::SetCmdList(const QStringList &cmdList)
{
    mCmdList = cmdList;
}

void SplitStickFrame::AddCmd(const QString &sCmd)
{
    mCmdList.append(sCmd);
}

void SplitStickFrame::Parse(const QString &sMsg, QList<SPLIT_FRAME> &frames)
{
    frames.clear();

    QString sRemainMsg = sMsg;
    QString firstMsg = "";
    SPLIT_FRAME frame;
    bool bMultiCmd = false;
    int multiCmdIndex = sRemainMsg.size();
    int index = 0;

    do{
        bMultiCmd = false;
        multiCmdIndex = sRemainMsg.size();
        index = 0;
        foreach(QString testCmd, mCmdList){
            index = sRemainMsg.indexOf(testCmd, 0, Qt::CaseInsensitive);
            if(index > 0){//不是在开头搜索到其他命令，说明有粘帧
                bMultiCmd = true;
                if(index < multiCmdIndex){//可能有多个命令粘帧，要找出最开头那个先
                    multiCmdIndex = index;
                }
            }
        }

        if(bMultiCmd){
            firstMsg = sRemainMsg.left(multiCmdIndex);
            sRemainMsg = sRemainMsg.mid(multiCmdIndex);
//            qDebug()<<"firstMsg"<<firstMsg;
//            qDebug()<<"sRemainMsg"<<sRemainMsg;
        }else{
            firstMsg = sRemainMsg;
            sRemainMsg = "";
        }

        QStringList orderPart = firstMsg.split(":");
        frame.sCmd = orderPart.at(0);
        orderPart.removeFirst();
        frame.sData = orderPart.join(":");
        frames.append(frame);
    }while(sRemainMsg != "");
}
