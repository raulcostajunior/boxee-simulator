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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBoxeeSignals()
{
    connect(&core::Boxee::instance(), &core::Boxee::stateChanged, [this](core::Boxee::State) {
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
    core::Boxee::State currState = core::Boxee::instance().state();
    ui->actionPower_On_Off->setEnabled(currState != core::Boxee::State::BOOTING
                                       && currState != core::Boxee::State::SHUTTING_DOWN);
    ui->actionInput_Text->setEnabled(currState == core::Boxee::State::ON_TEXT_ENTRY);
    bool frontFaceBtnsEnabled = (currState != core::Boxee::State::OFF
                                 && currState != core::Boxee::State::BOOTING
                                 && currState != core::Boxee::State::SHUTTING_DOWN);
    ui->actionUp->setEnabled(frontFaceBtnsEnabled);
    ui->actionDown->setEnabled(frontFaceBtnsEnabled);
    ui->actionLeft->setEnabled(frontFaceBtnsEnabled);
    ui->actionRight->setEnabled(frontFaceBtnsEnabled);
    ui->actionMenu->setEnabled(frontFaceBtnsEnabled);
    ui->actionEnter->setEnabled(frontFaceBtnsEnabled);
    ui->actionPlay_Pause->setEnabled(frontFaceBtnsEnabled);
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
