#ifndef MYTASKMENUEXTENSION_H
#define MYTASKMENUEXTENSION_H

#include <QObject>
#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>
#include <QTextStream>
#include <QFile>

class qhImage;
class MyTaskMenuExtension : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)

public:
    explicit MyTaskMenuExtension(qhImage *qi, QObject *parent);

    QAction *preferredEditAction() const override;
    QList<QAction *> taskActions() const override;

private slots:
    void editResource();
    void editFile();

private:
    QAction *selectResourceAction;
    QAction *selectFileAction;
    qhImage *qhi;

    QTextStream mqst;
    QFile mqf;

    void Save(const QString &name, const QString &value);
};


class MyTaskMenuFactory : public QExtensionFactory
{
    Q_OBJECT

public:
    explicit MyTaskMenuFactory(QExtensionManager *parent = nullptr);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const override;
};

#endif // MYTASKMENUEXTENSION_H
