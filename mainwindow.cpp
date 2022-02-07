#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h> // Required for winbase.h
#include <winbase.h> // Required for SetThreadExecutionState
#include <QTimer>
#include <QDesktopServices>
#include <QDateTime>
#include <string>
#include <QMessageBox>

using namespace std;

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
    ui->currentStatusLbl->setText("System is now in DEFAULT state :D!");

    //Init
    mouseTimer = new QTimer();
    mouseLastPos = QCursor::pos();
    mouseIdleSeconds = 0;
    lastIdleSec = 0;
    isForcedAwake = false;
    isNormal = true;
    QDateTime dt = dt.currentDateTime();
    ui->activeSinceLbl->setText("Active since "+dt.toString("yyyy-MM-dd HH:mm:ss"));

    //Connect and Start
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(onTick()));
    mouseTimer->start(1000);

    titleBarActions();
    sfnOnSwitch();
}

void MainWindow::onAwake(){
    isForcedAwake = true;
    isNormal = false;
    ui->currentStatusLbl->setText("System is now in FORCE AWAKE state :D!");
    ui->awakeBtn->setEnabled(false);
    ui->resetBtn->setEnabled(true);
}

void MainWindow::onAsleep(){
    isForcedAwake = false;
    isNormal = true;
    ui->currentStatusLbl->setText("System is now in DEFAULT state :D!");
    ui->awakeBtn->setEnabled(true);
    ui->resetBtn->setEnabled(false);
}

void MainWindow::onStateChange(){
    if (isForcedAwake){
        // The following sets the appropriate flags to prevent system to go into sleep mode.
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED | ES_DISPLAY_REQUIRED);
    }
    else{
        // This clears the flags and allows the system to sleep normally.
        SetThreadExecutionState(ES_CONTINUOUS);
    }
}

void MainWindow::onTick()
{
    QPoint point = QCursor::pos();
    if(point != mouseLastPos){
        if (mouseIdleSeconds != 0){lastIdleSec = mouseIdleSeconds;}
        mouseIdleSeconds = 0;
    }
    else{
        mouseIdleSeconds++;
    }

    mouseLastPos = point;
    onIdle();
    onStateChange();
}

void MainWindow::onIdle(){
    stringBuilder(mouseIdleSeconds, "Idle for", ui->windowStateLbl);
    stringBuilder(lastIdleSec, "Last idle:", ui->lastIdleLbl);
}

void MainWindow::stringBuilder(quint32 sec, std::string display, QLabel *label){
    QString result;
    quint32 tempSec = 0;
    if (sec >= 3600){
        tempSec = sec/3600;
        label->setText(result.asprintf("%s %d hour(s).",display.c_str(), tempSec));
    }
    else if (sec >= 60){
        tempSec = sec/60;
        label->setText(result.asprintf("%s %d minute(s).",display.c_str(), tempSec));
    }
    else{
        label->setText(result.asprintf("%s %d second(s).",display.c_str(), sec));
    }
}

void MainWindow::titleBarActions(){
    connect(ui->actionHow_to_use, SIGNAL(triggered()), this, SLOT(onOpenInfo()));
    connect(ui->actionSource_Codes, SIGNAL(triggered()), this, SLOT(onOpenUrl()));
    connect(ui->menuExit, SIGNAL(aboutToShow()), this, SLOT(onQuit()));
    connect(ui->actionStats_for_Nerd, SIGNAL(triggered()), this, SLOT(sfnOnSwitch()));
}

void MainWindow::onOpenInfo(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("How to use Force Awaken");
    msgBox.setWindowIcon(QIcon(":assets/logo.png"));
    msgBox.setText("Available action(s)");
    msgBox.setInformativeText("1. Force Awake: Prevent system from going into sleep/lock mode.\n2. Reset: Revert system to default settings.\n3. Stats for Nerd: Show/hide useless informations.");
    msgBox.exec();
}

void MainWindow::onOpenUrl(){
    QString link = "https://github.com/proscawards/force-awaken";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::onQuit(){
    onAsleep();
    QApplication::quit();
}

void MainWindow::sfnOnSwitch(){
    if (ui->actionStats_for_Nerd->isChecked()){
        ui->activeSinceLbl->setVisible(true);
        ui->lastIdleLbl->setVisible(true);
    }
    else{
        ui->activeSinceLbl->setVisible(false);
        ui->lastIdleLbl->setVisible(false);
    }
}

MainWindow::~MainWindow()
{
    //Reset thread back to normal on destruct
    onAsleep();
    delete ui;
}

