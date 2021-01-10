#include "qhsimpleselect.h"
#include "qhsimpleselectplugin.h"

#include <QtPlugin>
#include "../pluginver.h"

qhSimpleSelectPlugin::qhSimpleSelectPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void qhSimpleSelectPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool qhSimpleSelectPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *qhSimpleSelectPlugin::createWidget(QWidget *parent)
{
    return new qhSimpleSelect(parent);
}

QString qhSimpleSelectPlugin::name() const
{
    return QLatin1String("qhSimpleSelect");
}

QString qhSimpleSelectPlugin::group() const
{
    return QLatin1String(PLUGINVER);
}

QIcon qhSimpleSelectPlugin::icon() const
{
    return QIcon();
}

QString qhSimpleSelectPlugin::toolTip() const
{
    return QLatin1String("");
}

QString qhSimpleSelectPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool qhSimpleSelectPlugin::isContainer() const
{
    return false;
}

QString qhSimpleSelectPlugin::domXml() const
{
    return QLatin1String("<widget class=\"qhSimpleSelect\" name=\"qhsimpleselect\">\n</widget>\n");
}

QString qhSimpleSelectPlugin::includeFile() const
{
    return QLatin1String("qhsimpleselect.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qhsimpleselectplugin, qhSimpleSelectPlugin)
#endif // QT_VERSION < 0x050000
