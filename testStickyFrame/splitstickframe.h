#ifndef SPLITSTICKFRAME_H
#define SPLITSTICKFRAME_H

#include <QStringList>

typedef struct
{
    QString sCmd;
    QString sData;
}SPLIT_FRAME;

///
/// \brief SplitStickFrame::SplitStickFrame
/// 适用于解析字符流简单通信协议，例如协议格式为CMD:DATA1:DATA2，开头为命令号，接一个:分割符号，然后就是数据
/// 使用步骤：
/// 1、先把所有的CMD命令添加到mCmdList里
/// 2、然后调用Parse解析消息
/// 3、ACMD:data1:data2BCMD:data2，这样就粘帧了，可以解析为ACMD:data1:data2和BCMD:data1，当然限制是DATA段不能包含CMD段
///
class SplitStickFrame
{
public:
    SplitStickFrame();

    void SetCmdList(const QStringList &cmdList);
    void AddCmd(const QString &sCmd);

    void Parse(const QString &sMsg, QList<SPLIT_FRAME> &frames);

private:
    QStringList mCmdList;
};

#endif // SPLITSTICKFRAME_H
