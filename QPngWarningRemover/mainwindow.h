#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QTimer>
#include "pngremovetask.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void OnCheckThreadTimeout();

    void on_pushButton_clear_clicked();

    void on_pushButton_start_clicked();

private:
    Ui::MainWindow *ui;

    QTimer mTimerCheckThread;
    QStringList mPngFiles;
    qint32 mnThreadUsed;
    qint32 mnFileIndex;
    QMutex mMutex;
    QList<PngRemoveResult> mResult;

    qint64 mStartMSecs;
    qint64 mEndMSecs;

    bool AddFile(const QString &sFile);
    void StartThreads();
};

#endif // MAINWINDOW_H
