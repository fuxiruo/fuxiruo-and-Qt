#ifndef MYRESOURCEVIEWDIALOG_H
#define MYRESOURCEVIEWDIALOG_H

#include <QDialog>
#include <QMap>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QFile>
#include <QTextStream>

namespace Ui {
class MyResourceViewDialog;
}

class MyResourceViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyResourceViewDialog(QWidget *parent = nullptr);
    ~MyResourceViewDialog();

    void SetResourceFiles(const QStringList &qsl);
    QString GetResult() const;

private slots:
    void SlotCurrentPathChanged(QTreeWidgetItem *item);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MyResourceViewDialog *ui;
    QTextStream mqst;
    QFile mqf;

    QString mResult;

    QMap<QString, QStringList>       m_pathToContents; // full path to contents file names (full path to its resource filenames)
    QMap<QString, QString>           m_qrcDirToRealDir;
    QMap<QString, QString>           m_pathToParentPath; // full path to full parent path
    QMap<QString, QStringList>       m_pathToSubPaths; // full path to full sub paths
    QMap<QString, QTreeWidgetItem *> m_pathToItem;
    QMap<QTreeWidgetItem *, QString> m_itemToPath;
    QMap<QString, QListWidgetItem *> m_resourceToItem;
    QMap<QListWidgetItem *, QString> m_itemToResource;

    QPixmap MakeThumbnail(const QPixmap &pix) const;

    void CreatPath(const QString &resoureFile);
    QTreeWidgetItem *CreatePath(const QString &path, QTreeWidgetItem *parent);
    void CreateResources(const QString &path);
};

#endif // MYRESOURCEVIEWDIALOG_H
