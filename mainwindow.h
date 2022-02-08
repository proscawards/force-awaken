#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QLabel>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void onAwake();
    void onAsleep();
    void onIdle();
    void onStateChange();
    void titleBarActions();
    void stringBuilder(quint32, std::string, QLabel*);
    QString preprocessTimer(quint32);
    ~MainWindow();

public slots:
    void onTick();
    void onElapsed();
    void onOpenInfo();
    void onOpenUrl();
    void onQuit();
    void sfnOnSwitch();

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
};
#endif // MAINWINDOW_H
