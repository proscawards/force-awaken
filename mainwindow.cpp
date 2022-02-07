#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h> // Required for winbase.h
#include <winbase.h> // Required for SetThreadExecutionState
#include <QTimer>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(300,300));
    this->setWindowTitle("Force Awaken");
    this->setWindowIcon(QIcon(":assets/logo.png"));
    ui->copyrightLbl->setText("©Copyright 2022 proscawards. Made with ☕ and ❤️.");

    //Bind awake button to onAwake method
    connect(ui->awakeBtn, &QPushButton::released, this, &MainWindow::onAwake);

    //Bind reset button to onAsleep method
    connect(ui->resetBtn, &QPushButton::released, this, &MainWindow::onAsleep);

    //Disable reset button onInit
    ui->resetBtn->setEnabled(false);
    ui->currentStatusLbl->setText("System is now in normal state :D!");

    //Init
    mouseTimer = new QTimer();
    mouseLastPos = QCursor::pos();
    mouseIdleSeconds = 0;

    //Connect and Start
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(onTick()));
    mouseTimer->start(1000);

    titleBarActions();
}

void MainWindow::onAwake(){
    // The following sets the appropriate flags to prevent system to go into sleep mode.
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
    ui->currentStatusLbl->setText("System is now in force awake state :D!");
    ui->awakeBtn->setEnabled(false);
    ui->resetBtn->setEnabled(true);
}

void MainWindow::onAsleep(){
    // This clears the flags and allows the system to sleep normally.
    SetThreadExecutionState(ES_CONTINUOUS);
    ui->currentStatusLbl->setText("System is now in normal state :D!");
    ui->awakeBtn->setEnabled(true);
    ui->resetBtn->setEnabled(false);
}

void MainWindow::onTick()
{
    QPoint point = QCursor::pos();
    if(point != mouseLastPos)
        mouseIdleSeconds = 0;
    else
        mouseIdleSeconds++;

    mouseLastPos = point;
    onIdle();
}

void MainWindow::onIdle(){
    QString result;
    quint32 tempSec = 0;
    if (mouseIdleSeconds >= 3600){
        tempSec = mouseIdleSeconds/3600;
        ui->windowStateLbl->setText(result.asprintf("Idle for %d hour(s).", tempSec));
    }
    else if (mouseIdleSeconds >= 60){
        tempSec = mouseIdleSeconds/60;
        ui->windowStateLbl->setText(result.asprintf("Idle for %d minute(s).", tempSec));
    }
    else{
        ui->windowStateLbl->setText(result.asprintf("Idle for %d second(s).", mouseIdleSeconds));
    }
}

void MainWindow::titleBarActions(){
    connect(ui->actionSource_Codes, SIGNAL(triggered()), this, SLOT(onOpenUrl()));
    connect(ui->menuExit, SIGNAL(aboutToShow()), this, SLOT(onQuit()));
}

void MainWindow::onOpenUrl(){
    QString link = "https://github.com/proscawards/force-awaken";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::onQuit(){
    onAsleep();
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    //Reset thread back to normal on destruct
    onAsleep();
    delete ui;
}

