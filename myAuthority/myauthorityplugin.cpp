#include "myauthority.h"
#include "myauthorityplugin.h"

#include <QtPlugin>

MyAuthorityPlugin::MyAuthorityPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void MyAuthorityPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool MyAuthorityPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *MyAuthorityPlugin::createWidget(QWidget *parent)
{
    return new MyAuthority(parent);
}

QString MyAuthorityPlugin::name() const
{
    return QLatin1String("MyAuthority");
}

QString MyAuthorityPlugin::group() const
{
    return QLatin1String("");
}

QIcon MyAuthorityPlugin::icon() const
{
    return QIcon();
}

QString MyAuthorityPlugin::toolTip() const
{
    return QLatin1String("");
}

QString MyAuthorityPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool MyAuthorityPlugin::isContainer() const
{
    return false;
}

QString MyAuthorityPlugin::domXml() const
{
    return QLatin1String("<widget class=\"MyAuthority\" name=\"myAuthority\">\n</widget>\n");
}

QString MyAuthorityPlugin::includeFile() const
{
    return QLatin1String("myauthority.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(myauthorityplugin, MyAuthorityPlugin)
#endif // QT_VERSION < 0x050000
