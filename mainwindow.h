#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void titleBarActions();
    ~MainWindow();

public slots:
    void onTick();
    void onOpenUrl();
    void onQuit();

private:
    Ui::MainWindow *ui;
    QPoint mouseLastPos;
    QTimer *mouseTimer;
    quint32 mouseIdleSeconds;
};
#endif // MAINWINDOW_H
