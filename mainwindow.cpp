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
#include <QCloseEvent>

#ifndef QT_NO_EMIT
# define emit
#endif

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
    isEscape = false;
    elapsedTimer = new QTimer();
    QDateTime dt = QDateTime::currentDateTime();
    ui->activeSinceLbl->setText(dt.toString("yyyy-MM-dd HH:mm:ss"));
    event = new QCloseEvent();

    //Connect and Start
    connect(mouseTimer, SIGNAL(timeout()), this, SLOT(onTick()));
    mouseTimer->start(1000);

    //Connect and Start
    connect(elapsedTimer, SIGNAL(timeout()), this, SLOT(onElapsed()));
    elapsedTimer->start(1000);

    //System Tray
    QMenu *menu = new QMenu(this);
    createActions();
    menu->addAction(faAct);
    menu->addAction(reAct);
    menu->addSeparator();
    menu->addAction(sfnAct);
    menu->addAction(scAct);
    menu->addAction(htuAct);
    menu->addSeparator();
    menu->addAction(exAct);

    tray = new QSystemTrayIcon();
    tray->setIcon(QIcon(":assets/logo.png"));
    tray->setContextMenu(menu);
    tray->show();
    tray->activated(QSystemTrayIcon::DoubleClick);
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayOnTrigger(QSystemTrayIcon::ActivationReason)));

    titleBarActions();
    sfnOnSwitch();
}

//Setting up actions for system tray menu
void MainWindow::createActions(){
    //Force Awaken
    faAct = new QAction(tr("&Force Awake"), this);
    faAct->setShortcut(Qt::CTRL|Qt::Key_F);
    faAct->setStatusTip(tr("Prevent system from going into sleep/lock mode."));
    connect(faAct, &QAction::triggered, this, &MainWindow::onAwake);
    //Reset
    reAct = new QAction(tr("&Reset"), this);
    reAct->setShortcut(Qt::CTRL|Qt::Key_R);
    reAct->setStatusTip(tr("Revert system into default settings."));
    connect(reAct, &QAction::triggered, this, &MainWindow::onAsleep);
    //Exit
    exAct = new QAction(tr("&Exit"), this);
    exAct->setShortcut(Qt::Key_Escape);
    exAct->setStatusTip(tr("Exit application."));
    connect(exAct, &QAction::triggered, this, &MainWindow::onQuitPrompt);
    //Stats for nerd
    sfnAct = new QAction(tr("&Stats for Nerd"), this);
    sfnAct->setShortcut(Qt::Key_F1);
    sfnAct->setStatusTip(tr("Show/hide useless information."));
    connect(sfnAct, &QAction::triggered, this, &MainWindow::sfnOnSwitch);
    //Source codes
    scAct = new QAction(tr("&Source Codes"), this);
    scAct->setShortcut(Qt::CTRL|Qt::Key_G);
    scAct->setStatusTip(tr("Redirect to GitHub source codes."));
    connect(scAct, &QAction::triggered, this, &MainWindow::onOpenUrl);
    //How to use
    htuAct = new QAction(tr("&How to Use"), this);
    htuAct->setShortcut(Qt::CTRL|Qt::Key_H);
    htuAct->setStatusTip(tr("Simple guide on using the application."));
    connect(htuAct, &QAction::triggered, this, &MainWindow::onOpenInfo);
}

//Triggered when Force Awake is being clicked
void MainWindow::onAwake(){
    isForcedAwake = true;
    isNormal = false;
    ui->currentStatusLbl->setText("System is now in FORCE AWAKE state :D!");
    ui->awakeBtn->setEnabled(false);
    ui->resetBtn->setEnabled(true);
}

//Triggered when Reset is being clicked
void MainWindow::onAsleep(){
    isForcedAwake = false;
    isNormal = true;
    ui->currentStatusLbl->setText("System is now in DEFAULT state :D!");
    ui->awakeBtn->setEnabled(true);
    ui->resetBtn->setEnabled(false);
}

//Show/Hide system tray (notification) on clicked
void MainWindow::trayOnTrigger(QSystemTrayIcon::ActivationReason ar){
    if (ar == QSystemTrayIcon::Trigger){
        if (!this->isVisible()) {
            this->show();
        } else {
            this->hide();
        }
    }
}

//Triggered when Force Awake/Reset is clicked
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

//Triggered every seconds to capture user mouse input to validate if the user is idling
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

//Triggered every seconds since the launch of the application
void MainWindow::onElapsed()
{
    initSec++;
    ui->elapsedTimeLbl->setText(preprocessTimer(initSec));
}

//Called from onTick(), a wrapper to build string
void MainWindow::onIdle(){
    stringBuilder(mouseIdleSeconds, "Idle for", ui->windowStateLbl);
    stringBuilder(lastIdleSec, "", ui->lastIdleLbl);
}

//Custom function to build string for different labels
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

//Setting up actions for menu
void MainWindow::titleBarActions(){
    ui->actionForce_Awake->setShortcut(Qt::CTRL|Qt::Key_F);
    connect(ui->actionForce_Awake, SIGNAL(triggered()), this, SLOT(onAwake()));
    ui->actionReset->setShortcut(Qt::CTRL|Qt::Key_R);
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(onAsleep()));
    ui->actionHow_to_use->setShortcut(Qt::CTRL|Qt::Key_H);
    connect(ui->actionHow_to_use, SIGNAL(triggered()), this, SLOT(onOpenInfo()));
    ui->actionSource_Codes->setShortcut(Qt::CTRL|Qt::Key_G);
    connect(ui->actionSource_Codes, SIGNAL(triggered()), this, SLOT(onOpenUrl()));
    ui->actionStats_for_Nerd->setShortcut(Qt::Key_F1);
    connect(ui->actionStats_for_Nerd, SIGNAL(triggered()), this, SLOT(sfnOnSwitch()));
    ui->actionExit_to_system_tray->setShortcut(Qt::ALT|Qt::Key_F4);
    connect(ui->actionExit_to_system_tray, SIGNAL(triggered()), this, SLOT(onMinimizeToSysTray()));
    ui->actionExit_Application->setShortcut(Qt::Key_Escape);
    connect(ui->actionExit_Application, SIGNAL(triggered()), this, SLOT(onQuitPrompt()));
}

//Triggered when Alt+F4 is pressed
void MainWindow::onMinimizeToSysTray(){
    closeEvent(event);
}

//Open How to Use dialog
void MainWindow::onOpenInfo(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("How to use Force Awaken");
    msgBox.setWindowIcon(QIcon(":assets/logo.png"));
    msgBox.setText("Step(s):");
    msgBox.setInformativeText("1. Click on Force Awake (CTRL + F).<br/> 2. Force Awake is now enabled, to disable it, click on Reset (CTRL + R).<br/> 3. Reset is now enabled, to disable it, repeat from Step 1.");
    msgBox.exec();
}

//Redirect to GitHub
void MainWindow::onOpenUrl(){
    QString link = "https://github.com/proscawards/force-awaken";
    QDesktopServices::openUrl(QUrl(link));
}

//Prompt the user before quitting the application
void MainWindow::onQuitPrompt(){
    QMessageBox::StandardButton msgBox;
    msgBox = QMessageBox::warning(this, "Force Asleep", "Are you sure you want to quit?", QMessageBox::Yes | QMessageBox::No);
    if (msgBox == QMessageBox::Yes) {
        isEscape = true;
        onQuit();
    }
}

//Quit Application
void MainWindow::onQuit(){
    onAsleep();
    QApplication::quit();
}

//Show/Hide stats for nerd
void MainWindow::sfnOnSwitch(){
    if (ui->actionStats_for_Nerd->isChecked()){
        ui->sfnWidget->show();
    }
    else{
        ui->sfnWidget->hide();
    }
}

//Show app in front of the screen when desktop notification is clicked
void MainWindow::onMsgClick(){
    show();
}

//Preprocess seconds to dd:HH:mm:ss format
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

//Override QT default closeEvent (Close button & ALT+F4)
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (tray->isVisible()) {
        if (!isEscape){
            tray->showMessage("Force Awaken","Force Awaken is now running in background!",QIcon(":assets/logo.png"), 3000);
            connect(tray, SIGNAL(messageClicked()), this, SLOT(onMsgClick()));
        }
        hide();
        event->ignore();
    }
}

MainWindow::~MainWindow()
{
    //Reset thread back to normal on destruct
    onAsleep();
    delete ui;
}



