#include "qhmessageimage.h"
#include "qhmessageimageplugin.h"

#include <QtPlugin>
#include "../pluginver.h"

qhMessageImagePlugin::qhMessageImagePlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void qhMessageImagePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool qhMessageImagePlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *qhMessageImagePlugin::createWidget(QWidget *parent)
{
    return new qhMessageImage(parent);
}

QString qhMessageImagePlugin::name() const
{
    return QLatin1String("qhMessageImage");
}

QString qhMessageImagePlugin::group() const
{
    return QLatin1String(PLUGINVER);
}

QIcon qhMessageImagePlugin::icon() const
{
    return QIcon();
}

QString qhMessageImagePlugin::toolTip() const
{
    return QLatin1String("");
}

QString qhMessageImagePlugin::whatsThis() const
{
    return QLatin1String("");
}

bool qhMessageImagePlugin::isContainer() const
{
    return false;
}

QString qhMessageImagePlugin::domXml() const
{
    return QLatin1String("<widget class=\"qhMessageImage\" name=\"qhmessageimage\">\n</widget>\n");
}

QString qhMessageImagePlugin::includeFile() const
{
    return QLatin1String("qhmessageimage.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qhmessageimageplugin, qhMessageImagePlugin)
#endif // QT_VERSION < 0x050000
