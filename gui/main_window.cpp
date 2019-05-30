#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QTableView>
#include <QToolBar>

#include "core/boxee.h"
#include "main_window.h"
#include "ui_main_window.h"

#include "dto/net_message.h"

using namespace gui;
using namespace core;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initActions();
    initLogPanel();
    initStatusBar();

    updateActions();
    updateStatusBar();

    connectBoxeeSignals();

    this->setWindowTitle(tr("Boxee Simulator (%1)").arg(Boxee::instance().boxeeAddress()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBoxeeSignals()
{
    connect(&Boxee::instance(), &Boxee::stateChanged, [this](Boxee::State) {
        this->updateActions();
        this->updateStatusBar();
    });
}

void MainWindow::initActions()
{
    connect(ui->actionPower_On_Off, &QAction::triggered, this, &MainWindow::onPowerOnOff);
    connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
    connect(ui->actionAbout_Qt, &QAction::triggered, []() { qApp->aboutQt(); });
}

void MainWindow::updateActions()
{
    const Boxee::State currState = Boxee::instance().state();
    const Boxee::MediaType currMedia = Boxee::instance().mediaType();

    ui->actionPower_On_Off->setEnabled(currState != Boxee::State::BOOTING
                                       && currState != Boxee::State::SHUTTING_DOWN);
    if (currState != Boxee::State::OFF && currState != Boxee::State::BOOTING) {
        ui->actionPower_On_Off->setIcon(QIcon(":/images/power-off.png"));
    } else {
        ui->actionPower_On_Off->setIcon(QIcon(":/images/power-on.png"));
    }

    ui->actionInput_Text->setEnabled(currState == Boxee::State::ON_TEXT_ENTRY);

    bool frontFaceBtnsEnabled = (currState != Boxee::State::OFF
                                 && currState != Boxee::State::BOOTING
                                 && currState != Boxee::State::SHUTTING_DOWN);
    ui->actionUp->setEnabled(frontFaceBtnsEnabled);
    ui->actionDown->setEnabled(frontFaceBtnsEnabled);
    ui->actionLeft->setEnabled(frontFaceBtnsEnabled);
    ui->actionRight->setEnabled(frontFaceBtnsEnabled);
    ui->actionMenu->setEnabled(frontFaceBtnsEnabled);
    ui->actionEnter->setEnabled(frontFaceBtnsEnabled);

    ui->actionPlay_Pause->setEnabled(frontFaceBtnsEnabled && currMedia != Boxee::MediaType::NONE);
    if (currState == Boxee::State::ON_MEDIA_PLAYING) {
        ui->actionPlay_Pause->setIcon(QIcon(":/images/pause.png"));
    } else {
        ui->actionPlay_Pause->setIcon(QIcon(":/images/play.png"));
    }
}

void MainWindow::initLogPanel()
{
    dockLog = new QDockWidget(this);
    dockLog->setWindowTitle(tr("Communication Log"));
    dockLog->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->addDockWidget(Qt::BottomDockWidgetArea, dockLog);

    winLog = new QMainWindow(nullptr);
    toolbLog = new QToolBar(winLog);
    toolbLog->setMovable(false);
    toolbLog->setFloatable(false);
    toolbLog->setIconSize(
        QSize(ui->mainToolBar->iconSize().width() - 6, ui->mainToolBar->iconSize().height() - 6));
    toolbLog->addAction(ui->actionSave_As);
    toolbLog->addAction(ui->actionClear);
    winLog->addToolBar(toolbLog);

    tblLog = new QTableView();
    tblLog->setFrameStyle(QFrame::NoFrame);
    winLog->setCentralWidget(tblLog);
    winLog->setParent(dockLog);
    dockLog->setWidget(winLog);
}

void MainWindow::initStatusBar()
{
    lblBoxeeState = new QLabel("                       ");
    lblBoxeeState->setMinimumSize(lblBoxeeState->sizeHint());
    lblBoxeeState->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(lblBoxeeState);
}

void MainWindow::updateStatusBar()
{
    lblBoxeeState->setText(core::Boxee::instance().stateAsString());
}

void MainWindow::onPowerOnOff()
{
    if (core::Boxee::instance().state() == core::Boxee::State::OFF) {
        // Boxee will be turned on
        core::Boxee::instance().powerOn();
    } else {
        core::Boxee::instance().powerOff();
    }
}
