#ifndef CAPPINFO_H
#define CAPPINFO_H

#include <QString>

class CAPPInfo
{
public:
    CAPPInfo();

    static QString GetFileVersion(const QString &fullName, const QString &fieldName);
};

#endif // CAPPINFO_H
