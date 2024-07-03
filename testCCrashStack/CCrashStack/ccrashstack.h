/**
* @projectName
* @file          ccrashstack
* @brief         记录崩溃日志
* @date          2022-08-08
*/
 
#ifndef CCRASHSTACK_H
#define CCRASHSTACK_H
 
#include <windows.h>
#include <QString>
 
class CCrashStack
{
private:
    PEXCEPTION_POINTERS m_pException;
 
private:
    QString GetModuleByRetAddr(PBYTE Ret_Addr, PBYTE & Module_Addr);
    QString GetCallStack(PEXCEPTION_POINTERS pException);
    QString GetVersionStr();
    bool GetHardwareInaformation(QString &graphics_card, QString &sound_deivce);
 
public:
    CCrashStack(PEXCEPTION_POINTERS pException);
 
    QString GetExceptionInfo();
};
 
#endif // CCRASHSTACK_H