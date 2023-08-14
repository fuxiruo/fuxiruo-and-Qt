#include "myresourceviewdialog.h"
#include "ui_myresourceviewdialog.h"
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QDebug>
#include <QQueue>
#include <QPainter>

// Resource root up until 4.6 was ':', changed to ":/" as of 4.7
const QString root(QStringLiteral(":/"));

MyResourceViewDialog::MyResourceViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyResourceViewDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    mqf.setFileName("R:/output2.txt");
    if (mqf.open(QFile::WriteOnly | QFile::Truncate)) {
        mqst.setDevice(&mqf);
        mqst<<"open"<<endl;
    }

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                        this, SLOT(SlotCurrentPathChanged(QTreeWidgetItem*)));
}

MyResourceViewDialog::~MyResourceViewDialog()
{
    delete ui;
}

void MyResourceViewDialog::SetResourceFiles(const QStringList &qsl)
{
    mResult.clear();
    ui->treeWidget->clear();
    ui->listWidget->clear();

    m_pathToContents.clear();
    m_qrcDirToRealDir.clear();
    m_pathToParentPath.clear();
    m_pathToSubPaths.clear();

    foreach (QString s, qsl) {
        CreatPath(s);
    }
    QQueue<QPair<QString, QTreeWidgetItem *> > pathToParentItemQueue;
    pathToParentItemQueue.enqueue(qMakePair(root, static_cast<QTreeWidgetItem *>(0)));
    while (!pathToParentItemQueue.isEmpty()) {
        QPair<QString, QTreeWidgetItem *> pathToParentItem = pathToParentItemQueue.dequeue();
        const QString path = pathToParentItem.first;
        QTreeWidgetItem *item = CreatePath(path, pathToParentItem.second);
        //qDebug()<<path<<pathToParentItem.second;
        const QStringList subPaths = m_pathToSubPaths.value(path);
        for (const QString &subPath : subPaths)
            pathToParentItemQueue.enqueue(qMakePair(subPath, item));
    }
}

QString MyResourceViewDialog::GetResult() const
{
    return mResult;
}

void MyResourceViewDialog::SlotCurrentPathChanged(QTreeWidgetItem *item)
{
    mResult.clear();
    ui->listWidget->clear();
    m_resourceToItem.clear();
    m_itemToResource.clear();

    if (!item)
        return;

    const QString currentPath = m_itemToPath.value(item);
    CreateResources(currentPath);
}

QPixmap MyResourceViewDialog::MakeThumbnail(const QPixmap &pix) const
{
    int w = qMax(48, pix.width());
    int h = qMax(48, pix.height());
    QRect imgRect(0, 0, w, h);
    QImage img(w, h, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);
    if (!pix.isNull()) {
        QRect r(0, 0, pix.width(), pix.height());
        r.moveCenter(imgRect.center());
        QPainter p(&img);
        p.drawPixmap(r.topLeft(), pix);
    }
    return QPixmap::fromImage(img);
}

void MyResourceViewDialog::CreatPath(const QString &resoureFile)
{
    QFile file(resoureFile);
    QString prefix;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QXmlStreamReader xmlRead(&file);

            while (!xmlRead.atEnd())
            {

                xmlRead.readNext();
                if(xmlRead.isStartElement())
                {
                    if(xmlRead.name() == "qresource"){
                        if(xmlRead.attributes().hasAttribute("prefix")){
                            prefix = xmlRead.attributes().value("prefix").toString();
                        }
                    }else if(xmlRead.name() == "file"){
                        QString str = xmlRead.readElementText();
                        const QFileInfo qifResurce(resoureFile);
                        const QFileInfo qfi(prefix + str);
                        QString fullPath = qifResurce.path() + "/" + str;
                        const QFileInfo qfireal(fullPath);
                        QString dirPath = qfi.absolutePath();
                        dirPath.remove(0, 1);//移除C盘或者D盘符号
                        m_pathToContents[dirPath].append(qfi.fileName());
                        m_qrcDirToRealDir[dirPath] = qfireal.absolutePath();
                        mqst<<dirPath<<qfi.fileName()<<qfireal.absolutePath()<<endl;
                        qDebug()<<dirPath<<qfi.fileName()<<qfireal.absolutePath();

                        while (!m_pathToParentPath.contains(dirPath) && dirPath != root) { // create all parent paths
                            const QFileInfo fd(dirPath);
                            const QString parentDirPath = fd.absolutePath();
                            m_pathToParentPath[dirPath] = parentDirPath;
                            m_pathToSubPaths[parentDirPath].append(dirPath);
                            dirPath = parentDirPath;
                        }
                    }
                }
            }
        }
    }
}

QTreeWidgetItem *MyResourceViewDialog::CreatePath(const QString &path, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = 0;
    if (parent)
        item = new QTreeWidgetItem(parent);
    else
        item = new QTreeWidgetItem(ui->treeWidget);
    m_pathToItem[path] = item;
    m_itemToPath[item] = path;
    QString substPath;
    if (parent) {
        QFileInfo di(path);
        substPath = di.fileName();
    } else {
        substPath = QStringLiteral("<resource root>");
    }
    item->setText(0, substPath);
    item->setToolTip(0, path);
    return item;
}

void MyResourceViewDialog::CreateResources(const QString &path)
{
    const QStringList fileNames = m_pathToContents.value(path);
    for (const QString &fileName : fileNames) {
        QString filePath = m_qrcDirToRealDir[path] + "/" + fileName;
        QString qrcPath = path + "/" + fileName;

        mqst<<"CreateResources"<<filePath<<qrcPath<<endl;

        QFileInfo fi(filePath);
        if (fi.isFile()) {
            QListWidgetItem *item = new QListWidgetItem(fi.fileName(), ui->listWidget);
            const QPixmap pix = QPixmap(filePath);
            if (pix.isNull()) {
                item->setToolTip(qrcPath);
            } else {
                item->setIcon(QIcon(MakeThumbnail(pix)));
                const QSize size = pix.size();
                item->setToolTip(tr("Size: %1 x %2\n%3").arg(size.width()).arg(size.height()).arg(qrcPath));
            }
            item->setFlags(item->flags() | Qt::ItemIsDragEnabled);
            item->setData(Qt::UserRole, qrcPath);
            m_itemToResource[item] = qrcPath;
            m_resourceToItem[qrcPath] = item;
        }
    }
}

void MyResourceViewDialog::on_buttonBox_accepted()
{
    this->accept();
}

void MyResourceViewDialog::on_buttonBox_rejected()
{
    mResult.clear();
    this->reject();
}

void MyResourceViewDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString qrcPath = m_itemToResource[item];
    QFileInfo fi(qrcPath);
    QString qrcDir = fi.absolutePath();
    QString realDir = m_qrcDirToRealDir[qrcDir];

    mResult = qrcPath;
    qDebug()<<__FUNCTION__<<mResult;
}
