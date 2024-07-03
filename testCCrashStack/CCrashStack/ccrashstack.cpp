#include "ccrashstack.h"
#include <tlhelp32.h>
#include <stdio.h>
 
#define _WIN32_DCOM
#include <comdef.h>
#include <wbemidl.h>
 
//#include<base/constants.h>
#include "qdebug.h"
 
CCrashStack::CCrashStack(PEXCEPTION_POINTERS pException)
{
    m_pException = pException;
}
 
 
QString CCrashStack::GetModuleByRetAddr(PBYTE Ret_Addr, PBYTE & Module_Addr)
{
    MODULEENTRY32   M = {sizeof(M)};
    HANDLE  hSnapshot;
 
    wchar_t Module_Name[MAX_PATH] = {0};
 
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
 
    if ((hSnapshot != INVALID_HANDLE_VALUE) &&
            Module32First(hSnapshot, &M))
    {
            do
            {
                    if (DWORD(Ret_Addr - M.modBaseAddr) < M.modBaseSize)
                    {
                            lstrcpyn(Module_Name, M.szExePath, MAX_PATH);
                            Module_Addr = M.modBaseAddr;
                            break;
                    }
            } while (Module32Next(hSnapshot, &M));
    }
 
    CloseHandle(hSnapshot);
 
    QString sRet = QString::fromWCharArray(Module_Name);
    return sRet;
}
 
QString CCrashStack::GetCallStack(PEXCEPTION_POINTERS pException)
{
        PBYTE   Module_Addr_1;
        char bufer[256]={0};
        QString sRet;
 
        typedef struct STACK
        {
                STACK *Ebp;
                PBYTE  Ret_Addr;
                DWORD  Param[0];
        } STACK, *PSTACK;
 
        STACK   Stack = {0, 0};
        PSTACK  Ebp;
 
        if (pException)     //fake frame for exception address
        {
                Stack.Ebp = (PSTACK)pException->ContextRecord->Ebp;
                Stack.Ret_Addr = (PBYTE)pException->ExceptionRecord->ExceptionAddress;
                Ebp = &Stack;
        }
        else
        {
                Ebp = (PSTACK)&pException - 1;  //frame addr of Get_Call_Stack()
 
                // Skip frame of Get_Call_Stack().
                if (!IsBadReadPtr(Ebp, sizeof(PSTACK)))
                        Ebp = Ebp->Ebp;     //caller ebp
        }
 
        // Break trace on wrong stack frame.
        for (; !IsBadReadPtr(Ebp, sizeof(PSTACK)) && !IsBadCodePtr(FARPROC(Ebp->Ret_Addr));
                 Ebp = Ebp->Ebp)
        {
                // If module with Ebp->Ret_Addr found.
                memset(bufer,0, sizeof(bufer));
                sprintf(bufer, "\n%08X ", (Ebp->Ret_Addr));
                sRet.append(bufer);
 
                QString moduleName = this->GetModuleByRetAddr(Ebp->Ret_Addr, Module_Addr_1) ;
                if (moduleName.length() > 0)
                {
                    sRet.append(moduleName);
                }
        }
 
        return sRet;
} //Get_Call_Stack
 
QString CCrashStack::GetVersionStr()
{
        OSVERSIONINFOEX V = {sizeof(OSVERSIONINFOEX)};  //EX for NT 5.0 and later
 
        if (!GetVersionEx((POSVERSIONINFO)&V))
        {
                ZeroMemory(&V, sizeof(V));
                V.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                GetVersionEx((POSVERSIONINFO)&V);
        }
 
        if (V.dwPlatformId != VER_PLATFORM_WIN32_NT)
                V.dwBuildNumber = LOWORD(V.dwBuildNumber);  //for 9x HIWORD(dwBuildNumber) = 0x04xx
 
        QString sRet;
        sRet.append(QString("Windows:  %1.%2.%3, SP %4.%5, Product Type %6\n")
                .arg(V.dwMajorVersion).arg(V.dwMinorVersion).arg(V.dwBuildNumber)
                .arg(V.wServicePackMajor).arg(V.wServicePackMinor).arg(V.wProductType));
 
 
        return sRet;
}
 
QString CCrashStack::GetExceptionInfo()
{
        WCHAR       Module_Name[MAX_PATH];
        PBYTE       Module_Addr;
 
        QString sRet;
        char buffer[512]={0};
 
        QString sTmp = GetVersionStr();
        sRet.append(sTmp);
        sRet.append("Process:  ");
 
        GetModuleFileName(NULL, Module_Name, MAX_PATH);
        sRet.append(QString::fromWCharArray(Module_Name));
        sRet.append("\n");
 
        // If exception occurred.
        if (m_pException)
        {
                EXCEPTION_RECORD &  E = *m_pException->ExceptionRecord;
                CONTEXT &           C = *m_pException->ContextRecord;
 
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "Exception Addr: %08X  ", E.ExceptionAddress);
                sRet.append(buffer);
                // If module with E.ExceptionAddress found - save its path and date.
                QString module = GetModuleByRetAddr((PBYTE)E.ExceptionAddress, Module_Addr);
                if (module.length() > 0)
                {
                    sRet.append(" Module: ");
 
                    sRet.append(module);
                }
 
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "\nException Code:  %08X\n", (int)E.ExceptionCode);
                sRet.append(buffer);
 
                switch (E.ExceptionCode) {
                case EXCEPTION_ACCESS_VIOLATION:{
                    sRet.append("EXCEPTION_ACCESS_VIOLATION");
                    // Access violation type - Write/Read.
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer,"%s Address:  %08X\n",
                            (E.ExceptionInformation[0]) ? "Write" : "Read", (int)E.ExceptionInformation[1]);
                    sRet.append(buffer);
                }break;

                case EXCEPTION_DATATYPE_MISALIGNMENT:{
                    sRet.append("EXCEPTION_DATATYPE_MISALIGNMENT");
                }break;

                case EXCEPTION_INT_DIVIDE_BY_ZERO:{
                    sRet.append("EXCEPTION_INT_DIVIDE_BY_ZERO");
                }break;

                //其他错误

                default:
                    break;
                }
 
                sRet.append("\nInstruction: ");
                for (int i = 0; i < 16; i++)
                {
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, " %02X",  PBYTE(E.ExceptionAddress)[i]);
                    sRet.append(buffer);
                }
 
                sRet.append("\nRegisters: ");
 
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "\nRAX: %08X  RBX: %08X  RCX: %08X  RDX: %08X",  (unsigned int)C.Eax,(unsigned int) C.Ebx, (unsigned int)C.Ecx, (unsigned int)C.Edx);
                sRet.append(buffer);
 
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "\nRSI: %08X  RDI: %08X  RSP: %08X  RBP: %08X", (unsigned int)C.Esi, (unsigned int)C.Edi, (unsigned int)C.Esp, (unsigned int)C.Ebp);
                sRet.append(buffer);
 
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "\nRIP: %08X  RFlags: %08X", (unsigned int)C.Eip,(unsigned int) C.EFlags);
                sRet.append(buffer);
 
        } //if (pException)
 
        sRet.append("\nCall Stack:");
        QString sCallstack = this->GetCallStack(m_pException);
        sRet.append(sCallstack);
 
        return sRet;
}
 
#if 0
bool CCrashStack::GetHardwareInaformation(QString &graphics_card, QString &sound_deivce)
{
 
    HRESULT hres;
 
    // Initialize COM.
    hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    if (FAILED(hres))
    {
        qDebug() << "Failed to initialize COM library. "
                 << "Error code = 0x"
                 << hex << hres << endl;
        return false;
    }
 
    // Initialize
    hres = CoInitializeSecurity(NULL,
                                -1,
                                NULL,
                                NULL,
                                RPC_C_AUTHN_LEVEL_DEFAULT,
                                RPC_C_IMP_LEVEL_IMPERSONATE,
                                NULL,
                                EOAC_NONE,
                                NULL);
 
 
    if ((hres != RPC_E_TOO_LATE) && FAILED(hres))
    {
        qDebug() << "Failed to initialize security. "
                 << "Error code = 0x"
                 << hex << hres << endl;
        CoUninitialize();
        return false;
    }
 
    // Obtain the initial locator to Windows Management
    // on a particular host computer.
    IWbemLocator *pLoc = 0;
 
    hres = CoCreateInstance(
                CLSID_WbemLocator,
                0,
                CLSCTX_INPROC_SERVER,
                IID_IWbemLocator, (LPVOID *)&pLoc);
 
    if (FAILED(hres))
    {
        qDebug() << "Failed to create IWbemLocator object. "
                 << "Error code = 0x"
                 << hex << hres << endl;
        CoUninitialize();
        return false;
    }
 
    IWbemServices *pSvc = 0;
 
    // Connect to the root\cimv2 namespace with the
    // current user and obtain pointer pSvc
    // to make IWbemServices calls.
 
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
 
    if (FAILED(hres))
    {
        qDebug() << "Could not connect. Error code = 0x"
                 << hex << hres << endl;
        pLoc->Release();
        CoUninitialize();
        return false;
    }
 
    // Set the IWbemServices proxy so that impersonation
    // of the user (client) occurs.
    hres = CoSetProxyBlanket(
                pSvc,                         // the proxy to set
                RPC_C_AUTHN_WINNT,            // authentication service
                RPC_C_AUTHZ_NONE,             // authorization service
                NULL,                         // Server principal name
                RPC_C_AUTHN_LEVEL_CALL,       // authentication level
                RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
                NULL,                         // client identity
                EOAC_NONE                     // proxy capabilities
                );
 
    if (FAILED(hres))
    {
        qDebug() << "Could not set proxy blanket. Error code = 0x"
                 << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
 
 
    // Use the IWbemServices pointer to make requests of WMI.
    // Make requests here:
 
    IEnumWbemClassObject* pEnumerator_graphics = NULL;
    IEnumWbemClassObject* pEnumerator_sound = NULL;
    BSTR wql = SysAllocString(L"WQL");
    BSTR sql_graphics = SysAllocString(L"SELECT * FROM Win32_VideoController");
    BSTR sql_sound = SysAllocString(L"SELECT * FROM Win32_SoundDevice");
 
    HRESULT hres_graphics;
    HRESULT hres_sound;
    hres_graphics = pSvc->ExecQuery(wql,
                           sql_graphics,
                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                           NULL,
                           &pEnumerator_graphics);
    hres_sound = pSvc->ExecQuery(wql,
                           sql_sound,
                           WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                           NULL,
                           &pEnumerator_sound);
 
    if (FAILED(hres_graphics) || FAILED(hres_sound))
    {
        qDebug() << "Query for processes failed. "
                 << "Error code = 0x"
                 << hex << hres_graphics << endl;
 
        qDebug() << "Query for processes failed. "
                 << "Error code = 0x"
                 << hex << hres_sound << endl;
 
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return "";
    }
    else
    {
        IWbemClassObject *pclsObj_graphics;
        ULONG uReturnGraphics = 0;
 
        IWbemClassObject *pclsObj_sound;
        ULONG uReturnSound = 0;
 
        while (pEnumerator_graphics)
        {
            hres_graphics = pEnumerator_graphics->Next(WBEM_INFINITE, 1, &pclsObj_graphics, &uReturnGraphics);
 
            if (0 == uReturnGraphics)
            {
                break;
            }
 
            VARIANT description_vtProp;
            VARIANT driver_version_vtProp;
            VARIANT status_vtProp;
 
            // Get the value of the Name property
            hres_graphics = pclsObj_graphics->Get(L"Description", 0, &description_vtProp, 0, 0);
            QString description((QChar*) description_vtProp.bstrVal, wcslen(description_vtProp.bstrVal));
 
            hres_graphics = pclsObj_graphics->Get(L"DriverVersion", 0, &driver_version_vtProp, 0, 0);
            QString driver_version((QChar*) driver_version_vtProp.bstrVal, wcslen(driver_version_vtProp.bstrVal));
 
            hres_graphics = pclsObj_graphics->Get(L"Status", 0, &status_vtProp, 0, 0);
            QString status((QChar*) status_vtProp.bstrVal, wcslen(status_vtProp.bstrVal));
 
            graphics_card.append(description).append(" ").append(driver_version).append(" ").append(status).append(" \n");
 
            VariantClear(&description_vtProp);
            VariantClear(&driver_version_vtProp);
            VariantClear(&status_vtProp);
        }
 
        while (pEnumerator_sound)
        {
            hres_sound = pEnumerator_sound->Next(WBEM_INFINITE, 1, &pclsObj_sound, &uReturnSound);
 
            if (0 == uReturnSound)
            {
                break;
            }
 
            VARIANT description_vtProp;
            VARIANT status_vtProp;
 
            // Get the value of the Name property
            hres_sound = pclsObj_sound->Get(L"Description", 0, &description_vtProp, 0, 0);
            QString description((QChar*) description_vtProp.bstrVal, wcslen(description_vtProp.bstrVal));
 
            hres_sound = pclsObj_sound->Get(L"Status", 0, &status_vtProp, 0, 0);
            QString status((QChar*) status_vtProp.bstrVal, wcslen(status_vtProp.bstrVal));
 
            sound_deivce.append(description).append(" ").append(status).append(" \n");
 
            VariantClear(&description_vtProp);
            VariantClear(&status_vtProp);
        }
    }
 
    // Cleanup
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
    return true;
}
#endif
 
