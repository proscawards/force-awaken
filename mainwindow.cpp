#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h> // Required for winbase.h
#include <winbase.h> // Required for SetThreadExecutionState
#include <QTimer>
#include <QDesktopServices>
#include <QDateTime>
#include <string>
#include <QMessageBox>
#include <QShortcut>

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
    ui->awakeBtn->setShortcut(Qt::CTRL|Qt::Key_F);
    connect(ui->awakeBtn, &QPushButton::released, this, &MainWindow::onAwake);

    //Bind reset button to onAsleep method
    ui->resetBtn->setShortcut(Qt::CTRL|Qt::Key_R);
    connect(ui->resetBtn, &QPushButton::released, this, &MainWindow::onAsleep);

    //Disable reset button onInit
    ui->resetBtn->setEnabled(false);
    ui->currentStatusLbl->setText("System is now in DEFAULT state :D!");

    //Init
    mouseTimer = new QTimer();
    mouseLastPos = QCursor::pos();
    mouseIdleSeconds = 0;
    lastIdleSec = 0;
    initSec = 0;
    isForcedAwake = false;
    isNormal = true;
    elapsedTimer = new QTimer();
    QDateTime dt = QDateTime::currentDateTime();
    ui->activeSinceLbl->setText(dt.toString("yyyy-MM-dd HH:mm:ss"));

    //Connect and Start
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(onTick()));
    mouseTimer->start(1000);

    //Connect and Start
    connect(elapsedTimer, SIGNAL(timeout()), this, SLOT(onElapsed()));
    elapsedTimer->start(1000);

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
    ui->idleTimeLbl->setText(preprocessTimer(mouseIdleSeconds));
    mouseLastPos = point;
    onIdle();
    onStateChange();
}

void MainWindow::onElapsed()
{
    initSec++;
    ui->elapsedTimeLbl->setText(preprocessTimer(initSec));
}

void MainWindow::onIdle(){
    stringBuilder(mouseIdleSeconds, "Idle for", ui->windowStateLbl);
    stringBuilder(lastIdleSec, "", ui->lastIdleLbl);
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
    ui->actionHow_to_use->setShortcut(Qt::CTRL|Qt::Key_H);
    connect(ui->actionHow_to_use, SIGNAL(triggered()), this, SLOT(onOpenInfo()));
    ui->actionSource_Codes->setShortcut(Qt::CTRL|Qt::Key_G);
    connect(ui->actionSource_Codes, SIGNAL(triggered()), this, SLOT(onOpenUrl()));
    connect(ui->menuExit, SIGNAL(aboutToShow()), this, SLOT(onQuit()));
    ui->actionStats_for_Nerd->setShortcut(Qt::Key_F1);
    connect(ui->actionStats_for_Nerd, SIGNAL(triggered()), this, SLOT(sfnOnSwitch()));
}

void MainWindow::onOpenInfo(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("How to use Force Awaken");
    msgBox.setWindowIcon(QIcon(":assets/logo.png"));
    msgBox.setText("Available action(s)");
    msgBox.setInformativeText("1. Force Awake (Ctrl+F): Prevent system from going into sleep/lock mode.\n2. Reset (Ctrl+R): Revert system to default settings.\n3. Stats for Nerd (F1): Show/hide useless informations.");
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
        ui->sfnWidget->show();
    }
    else{
        ui->sfnWidget->hide();
    }
}

QString MainWindow::preprocessTimer(quint32 duration)
{
  QString res;
  int seconds = (int) (duration % 60);
  duration /= 60;
  int minutes = (int) (duration % 60);
  duration /= 60;
  int hours = (int) (duration % 24);
  int days = (int) (duration / 24);
  if (days == 0)
      return res.asprintf("%02d:%02d:%02d", hours, minutes, seconds);
  return res.asprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

MainWindow::~MainWindow()
{
    //Reset thread back to normal on destruct
    onAsleep();
    delete ui;
}

