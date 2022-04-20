#include "cappinfo.h"
#include "windows.h"

CAPPInfo::CAPPInfo()
{

}

QString CAPPInfo::GetFileVersion(const QString &fullName , const QString &fieldName)
{
    QString result = "";
    char* pData = nullptr;
    do
    {
        DWORD dwLen = GetFileVersionInfoSize(fullName.toStdWString().c_str(), 0);
        if (0 == dwLen)
        {
            break;
        }
        pData = new char [dwLen+1];
        BOOL bSuccess = GetFileVersionInfo(fullName.toStdWString().c_str(), 0, dwLen, pData);
        if (!bSuccess)
        {
            break;
        }
        struct LANGANDCODEPAGE
        {
          WORD wLanguage;
          WORD wCodePage;
        } *lpTranslate;
        LPVOID lpBuffer = nullptr;
        UINT uLen = 0;
        bSuccess = VerQueryValue(pData, (TEXT("\\VarFileInfo\\Translation")), (LPVOID*)&lpTranslate, &uLen);
        if(!bSuccess)
        {
            break;
        }
        QString str1, str2;
        str1.setNum(lpTranslate->wLanguage, 16);
        str2.setNum(lpTranslate->wCodePage, 16);
        str1 = "000" + str1;
        str2 = "000" + str2;
        QString verPath = "\\StringFileInfo\\"+ str1.right(4) + str2.right(4) +"\\" + fieldName;
        bSuccess = VerQueryValue(pData, (verPath.toStdWString().c_str()), &lpBuffer, &uLen);
        if(!bSuccess)
        {
            break;
        }
        result = QString::fromUtf16((const unsigned short int *)lpBuffer);
    }while(false);
    if(nullptr != pData)
    {
        delete pData;
    }
    return result;
}
