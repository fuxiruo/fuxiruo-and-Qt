#include "qhimage.h"
#include "qhimageplugin.h"

#include <QtPlugin>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>
#include "mytaskmenuextension.h"

qhImagePlugin::qhImagePlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void qhImagePlugin::initialize(QDesignerFormEditorInterface * formEditor)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here
    QExtensionManager *manager = formEditor->extensionManager();
    Q_ASSERT(manager != 0);

    manager->registerExtensions(new MyTaskMenuFactory(manager),
                              Q_TYPEID(QDesignerTaskMenuExtension));

    m_initialized = true;
}

bool qhImagePlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *qhImagePlugin::createWidget(QWidget *parent)
{
    return new qhImage(parent);
}

QString qhImagePlugin::name() const
{
    return QLatin1String("qhImage");
}

QString qhImagePlugin::group() const
{
    return QLatin1String("");
}

QIcon qhImagePlugin::icon() const
{
    return QIcon();
}

QString qhImagePlugin::toolTip() const
{
    return QLatin1String("");
}

QString qhImagePlugin::whatsThis() const
{
    return QLatin1String("");
}

bool qhImagePlugin::isContainer() const
{
    return false;
}

QString qhImagePlugin::domXml() const
{
    return QLatin1String("<widget class=\"qhImage\" name=\"qhimage\">\n</widget>\n");
}

QString qhImagePlugin::includeFile() const
{
    return QLatin1String("qhimage.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qhimageplugin, qhImagePlugin)
#endif // QT_VERSION < 0x050000
