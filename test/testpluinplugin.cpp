#include "testpluin.h"
#include "testpluinplugin.h"

#include <QtPlugin>

TestPluinPlugin::TestPluinPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void TestPluinPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool TestPluinPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TestPluinPlugin::createWidget(QWidget *parent)
{
    return new TestPluin(parent);
}

QString TestPluinPlugin::name() const
{
    return QLatin1String("TestPluin");
}

QString TestPluinPlugin::group() const
{
    return QLatin1String("");
}

QIcon TestPluinPlugin::icon() const
{
    return QIcon();
}

QString TestPluinPlugin::toolTip() const
{
    return QLatin1String("");
}

QString TestPluinPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool TestPluinPlugin::isContainer() const
{
    return false;
}

QString TestPluinPlugin::domXml() const
{
    return QLatin1String("<widget class=\"TestPluin\" name=\"testPluin\">\n</widget>\n");
}

QString TestPluinPlugin::includeFile() const
{
    return QLatin1String("testpluin.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(testpluinplugin, TestPluinPlugin)
#endif // QT_VERSION < 0x050000
