#include "mytaskmenuextension.h"
#include <QAction>
#include <QtDesigner/QDesignerFormWindowInterface>
#include <QtDesigner/QDesignerFormWindowCursorInterface>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QFileInfo>
#include <QtDesigner/QAbstractFormBuilder>
#include <QFileDialog>
#include "qhimage.h"
#include "myresourceviewdialog.h"

MyTaskMenuExtension::MyTaskMenuExtension(qhImage *qi, QObject *parent)
    : QObject(parent)
    , selectResourceAction(new QAction(tr("Select Resource"), this))
    , selectFileAction(new QAction(tr("Select File"), this))
    , qhi(qi)
{
    mqf.setFileName("R:/output.txt");
    if (mqf.open(QFile::WriteOnly | QFile::Truncate)) {
        mqst.setDevice(&mqf);
        mqst<<"open"<<endl;
    }
    connect(selectResourceAction, &QAction::triggered, this, &MyTaskMenuExtension::editResource);
    connect(selectFileAction, &QAction::triggered, this, &MyTaskMenuExtension::editFile);
}

QAction *MyTaskMenuExtension::preferredEditAction() const
{
    return selectResourceAction;
}

QList<QAction *> MyTaskMenuExtension::taskActions() const
{
    return QList<QAction *>{selectResourceAction, selectFileAction};
}

void MyTaskMenuExtension::editResource()
{
    if (QDesignerFormWindowInterface *fw = QDesignerFormWindowInterface::findFormWindow(qhi)){
        mqst<<"editResource"<<endl;
        foreach(QString s, fw->activeResourceFilePaths()){
            mqst<<s<<endl;
        }
        MyResourceViewDialog mrd;
        mrd.SetResourceFiles(fw->activeResourceFilePaths());
        mrd.exec();
        if(!mrd.GetResult().isEmpty()){
            Save("PicPath", mrd.GetResult());
        }
    }
}

void MyTaskMenuExtension::editFile()
{
    //mqst<<"editFile"<<endl;
    QString fileName = QFileDialog::getOpenFileName(nullptr,
          tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty()){
        Save("PicPath", fileName);
    }
}

void MyTaskMenuExtension::Save(const QString &name, const QString &value)
{
    if (QDesignerFormWindowInterface *formWindow
            = QDesignerFormWindowInterface::findFormWindow(qhi)) {
        formWindow->cursor()->setProperty(name, value);
    }
}

MyTaskMenuFactory::MyTaskMenuFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{

}

QObject *MyTaskMenuFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return nullptr;

    if (qhImage *tic = qobject_cast<qhImage*>(object))
        return new MyTaskMenuExtension(tic, parent);

    return nullptr;
}
