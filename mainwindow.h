#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QLabel>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSignalMapper>
#include "storage.h"
#include <QNetworkRequest>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void onIdle();
    void onQuit();
    void onStateChange();
    void titleBarActions();
    void stringBuilder(quint32, std::string, QLabel*);
    QString preprocessTimer(quint32);
    void updateUi();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onAwake();
    void onAsleep();
    void onTick();
    void onElapsed();
    void onOpenInfo();
    void onOpenUrl();
    void onQuitPrompt();
    void sfnOnSwitch();
    void trayOnTrigger(QSystemTrayIcon::ActivationReason);
    void createActions();
    void onMinimizeToSysTray();
    void onMsgClick();
    void onFaIdleOnly();
    void onFaInit();
    void onCheckUpdate();
    void onRedirectUpdate();
    void onAutoboot();

private:
    Ui::MainWindow *ui;
    QPoint mouseLastPos;
    QTimer *mouseTimer;
    quint32 mouseIdleSeconds;
    quint32 lastIdleSec;
    quint32 initSec;
    QTimer *elapsedTimer;
    bool isForcedAwake;
    bool isNormal;
    bool isEscape;
    QSystemTrayIcon *tray;
    QAction *faAct;
    QAction *reAct;
    QAction *exAct;
    QAction *sfnAct;
    QAction *scAct;
    QAction *htuAct;
    QAction *cfuAct;
    QCloseEvent *event;
    Storage *s;
    QString latestVer;
};
#endif // MAINWINDOW_H
