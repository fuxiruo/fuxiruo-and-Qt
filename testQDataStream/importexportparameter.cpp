#include "importexportparameter.h"
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QVector>
#include <QCryptographicHash>

//导入导出参数包用
#define QDataStream_Version QDataStream::Qt_5_9
/*文件头*/
#define PARAM_HEAD      0xAA55AA55
/*文件版本10 aa bb cc，文件格式跟ParameterDetail结构有关，
cc为次版本号，Parameter新增字段后变更此版本号(兼容),导入时可以兼容旧版本导出文件，只是旧的导出文件缺失新增字段需要跳过
bb为主版本号，Parameter有不兼容变更时变更此版本号，导入旧版本提示不兼容
aa为保留版本号，例如，校验码函数或者QDataStream的version发生变更时可以变更此版本号*/
#define PARAM_VERSION   0x10000001
#define PARAM_MAJOR_MASK 0xFFFFFF00
#define PARAM_MINOR_MASK (~PARAM_MAJOR_MASK)
#define PARAM_MAJOR_VERSION   (PARAM_VERSION&PARAM_MAJOR_MASK)
#define PARAM_MINOR_VERSION   (PARAM_VERSION&PARAM_MINOR_MASK)

ImportExportParameter::ImportExportParameter(QObject *parent) : QObject(parent)
{

}

bool ImportExportParameter::Export(const QString &sFile, const QVector<Parameter> &v)
{
    /*1、写入参数到文件*/
    QFile file(sFile);
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream out(&file);

    // Write a header with a "magic number" and a version
    out << (quint32)PARAM_HEAD;
    out << (quint32)PARAM_VERSION;

    out.setVersion(QDataStream_Version);

    out << (quint32)v.size();//参数的数量

    foreach (Parameter p, v) {
        out << p.str;               //参数包名
        out << p.uNum;             //控件所在widget名称
    }
    file.close();

     /*2、尾部追加MD5*/
    if(!file.open(QIODevice::ReadWrite)){
        return false;
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(&file);  // 添加数据到加密哈希值
    qDebug()<<hash.result().toHex();

    file.seek(file.size());
    QDataStream out2(&file);
    qDebug()<<out2.atEnd();
    out2 << hash.result().toHex();
    file.close();

    return true;
}

ImportExportParameter::ErrCode ImportExportParameter::Import(const QString &sFile, QVector<Parameter> &v)
{
    ErrCode errCode = ErrCode::OK;

    QFile file(sFile);
    if(!file.open(QIODevice::ReadOnly)){
        return ErrCode::Unknown;
    }
    QDataStream in(&file);

    // Read and check the header
    quint32 head;
    in >> head;
    if (head != PARAM_HEAD)
      return ErrCode::ErrHead;

    quint32 version;
    in >> version;
    quint32 major,minor;
    qDebug()<<"version:"<<QString::number(version, 16).toUpper();
    major = version & PARAM_MAJOR_MASK;
    minor = version & PARAM_MINOR_MASK;
    qDebug()<<"major version:"<<QString::number(major, 16).toUpper()<<"<>"<<QString::number(PARAM_MAJOR_VERSION, 16).toUpper();
    qDebug()<<"minor version:"<<QString::number(minor, 16).toUpper()<<"<>"<<QString::number(PARAM_MINOR_VERSION, 16).toUpper();
    if(version != PARAM_VERSION){
        if(major != PARAM_MAJOR_VERSION){
            qWarning()<<QString::number(major, 16).toUpper()<<"!="<<QString::number(PARAM_MAJOR_VERSION, 16).toUpper();
            return ErrCode::ErrVersion;
        }
    }


    if(in.atEnd()){
        qWarning()<<"empty data";
        return ErrCode::ErrData;
    }

    quint32 nSize;
    in >> nSize;
    for(quint32 i=0; i<nSize; i++){
        if(in.atEnd()){
            qWarning()<<"data size err";
            return ErrCode::ErrData;
        }

        Parameter p;
        in >> p.str;
        qDebug()<<p.str;
        in >> p.uNum;
        qDebug()<<p.uNum;
        v.append(p);
    }


    //读入MD5
    QByteArray qbMD5;
    in >> qbMD5;
    qDebug()<<qbMD5;

    file.close();

    //计算文件MD5
    if(!file.open(QIODevice::ReadOnly)){
        return ErrCode::Unknown;
    }
    QByteArray qbRead;
    QCryptographicHash hash(QCryptographicHash::Md5);
    int nPrevRead = 1024;
    int nTailSize = 4+32;//最后36个字节是4字节大小、MD5字符串，上面计算MD5时其实也把结束符+计算了，所以这里也要把最后的结束符+算进来
    while(file.bytesAvailable() > nTailSize){
        qbRead.clear();
        if(file.bytesAvailable() > nPrevRead+nTailSize){
            qbRead = file.read(nPrevRead);//分开读取，防止文件太大
        }else{
            qbRead = file.read(file.bytesAvailable()-nTailSize);
        }
        //qDebug()<<qbRead.toHex(' ');
        hash.addData(qbRead);  // 添加数据到加密哈希值
    }
    qDebug()<<hash.result().toHex();
    file.close();

    if(qbMD5 != hash.result().toHex()){
        qWarning()<<"MD5 err";
        return ErrCode::ErrVerify;
    }

    return errCode;
}
