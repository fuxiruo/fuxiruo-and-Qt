#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QFileInfo>
#include <QDir>
#include <QThreadPool>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();

    QThreadPool::globalInstance()->setMaxThreadCount(50);
    ui->spinBox_max_thread->setMaximum(QThreadPool::globalInstance()->maxThreadCount());
    ui->spinBox_max_thread->setValue(QThread::idealThreadCount());

    ui->groupBox_wait->installEventFilter(this);

    mTimerCheckThread.setInterval(50);
    connect(&mTimerCheckThread, SIGNAL(timeout()), this, SLOT(OnCheckThreadTimeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->groupBox_wait){
        if(event->type() == QEvent::DragEnter){
            QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent*>(event);
            if(dragEnterEvent->mimeData()->hasUrls()){
                QList<QUrl> urls = dragEnterEvent->mimeData()->urls();
                if(!urls.isEmpty()){
                    QString sPath = urls.first().toLocalFile();
                    QFileInfo qfi(sPath);
                    if(qfi.isFile() && qfi.suffix().toUpper()=="PNG"){
                        dragEnterEvent->acceptProposedAction();
                    }else if(qfi.isDir()){
                        dragEnterEvent->acceptProposedAction();
                    }
                }
            }
        }else if(event->type() == QEvent::Drop){
            QDropEvent *dragEnterEvent = static_cast<QDropEvent*>(event);
            if(dragEnterEvent->mimeData()->hasUrls()){
                QList<QUrl> urls = dragEnterEvent->mimeData()->urls();
                if(!urls.isEmpty()){
                    QString sPath = urls.first().toLocalFile();
                    QFileInfo qfi(sPath);
                    if(qfi.isFile()){
                        AddFile(sPath);
                    }else if(qfi.isDir()){
                        QList<QString> listDirs;
                        listDirs.append(sPath);
                        while(listDirs.size() > 0){
                            QString s = listDirs.takeFirst();
                            QDir qdir(s);
                            qdir.setFilter(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
                            QFileInfoList rootList = qdir.entryInfoList();
                            for (int i = 0; i < rootList.size(); ++i) {
                                QFileInfo fileInfo = rootList.at(i);
                                if(fileInfo.isDir()){
                                    listDirs.append(fileInfo.filePath());
                                }else if(fileInfo.suffix().toUpper()=="PNG"){
                                    AddFile(fileInfo.filePath());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void MainWindow::OnCheckThreadTimeout()
{
    if(mMutex.tryLock(10)){
        while(mResult.size() > 0){
            PngRemoveResult result = mResult.takeFirst();
            QString s = result.file;
            if(!result.result.isEmpty()){
                s += QString("(%1)").arg(result.result);
            }
            ui->listWidget_done->addItem(s);

            mnThreadUsed--;
        }
        mMutex.unlock();

        if(ui->progressBar->value() != ui->listWidget_done->count()){
            ui->progressBar->setValue(ui->listWidget_done->count());
            ui->label_done_count->setText(QString::number(ui->listWidget_done->count()));
        }
        StartThreads();
    }else{
        qWarning()<<__PRETTY_FUNCTION__<<"tryLock fail";
    }

    if(ui->listWidget_done->count() == ui->listWidget_wait->count()){
        mEndMSecs = QDateTime::currentMSecsSinceEpoch();
        mTimerCheckThread.stop();
        ui->progressBar->hide();
        ui->pushButton_clear->setEnabled(true);
        ui->pushButton_start->setEnabled(true);
        ui->statusBar->showMessage(QString("转换完成(%1 ms)").arg(mEndMSecs-mStartMSecs));
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    mPngFiles.clear();
    ui->listWidget_wait->clear();
    ui->listWidget_done->clear();
    ui->label_wait_count->clear();
    ui->label_done_count->clear();
}

bool MainWindow::AddFile(const QString &sFile)
{
    if(mPngFiles.contains(sFile)){
        return false;
    }else{
        mPngFiles.append(sFile);
        ui->listWidget_wait->addItem(sFile);
        ui->label_wait_count->setText(QString::number(mPngFiles.size()));
        return true;
    }
}

void MainWindow::StartThreads()
{
    while(mnFileIndex < mPngFiles.size()){
        if(mnThreadUsed < ui->spinBox_max_thread->value()){
            PngRemoveTask *p = new PngRemoveTask();
            p->SetArg(mPngFiles.at(mnFileIndex));
            p->SetMutex(&mMutex, &mResult);
            QThreadPool::globalInstance()->start(p);
            mnThreadUsed++;
            mnFileIndex++;
        }else{
            break;
        }
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    ui->listWidget_done->clear();
    ui->progressBar->setMaximum(mPngFiles.size());
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    mTimerCheckThread.start();
    ui->statusBar->showMessage("开始转换");

    mnThreadUsed = 0;
    mnFileIndex = 0;
    mStartMSecs = QDateTime::currentMSecsSinceEpoch();
    ui->pushButton_clear->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    StartThreads();
}
