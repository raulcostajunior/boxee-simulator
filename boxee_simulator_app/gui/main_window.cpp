#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QToolBar>

#include "core/boxee.h"
#include "core/virtual_remote.h"
#include "gui/log_view.h"
#include "gui/main_window.h"
#include "gui/preferences_dialog.h"
#include "model/net_message.h"
#include "ui_main_window.h"

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

    connectSignals();

    loadSettings();

    setWindowIcon(QIcon(":/images/appIcon_256.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals()
{
    connect(&Boxee::instance(), &Boxee::stateChanged, [this](Boxee::State) {
        this->updateActions();
        this->updateStatusBar();
    });

    connect(tblLog, &LogView::logChanged, [this] { this->updateActions(); });
}

void MainWindow::initActions()
{
    connect(ui->actionPower_On_Off, &QAction::triggered, this, &MainWindow::onPowerOnOff);
    connect(ui->actionQuit, &QAction::triggered, [this] { this->close(); });
    connect(ui->actionAbout_Qt, &QAction::triggered, [] { qApp->aboutQt(); });
    connect(ui->actionClear, &QAction::triggered, [this] { this->tblLog->clearLog(); });
    connect(ui->actionSettings, &QAction::triggered, [this] {
        PreferencesDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            this->updateStatusBar();
            this->saveSettings();
        }
    });

    // VirtualRemote actions
    connect(ui->actionUp, &QAction::triggered, [] { VirtualRemote::instance().sendUp(); });
    connect(ui->actionRight, &QAction::triggered, [] { VirtualRemote::instance().sendRight(); });
    connect(ui->actionDown, &QAction::triggered, [] { VirtualRemote::instance().sendDown(); });
    connect(ui->actionLeft, &QAction::triggered, [] { VirtualRemote::instance().sendLeft(); });
    connect(ui->actionEnter, &QAction::triggered, [] { VirtualRemote::instance().sendEnter(); });
    connect(ui->actionMenu, &QAction::triggered, [] { VirtualRemote::instance().sendMenu(); });
    connect(ui->actionPlay_Pause, &QAction::triggered, [] {
        VirtualRemote::instance().sendPlayPause();
    });
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

    ui->actionClear->setEnabled(!tblLog->isEmpty());
    ui->actionSave_As->setEnabled(!tblLog->isEmpty());

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

    tblLog = new LogView(nullptr);
    tblLog->setFrameStyle(QFrame::NoFrame);
    winLog->setCentralWidget(tblLog);
    winLog->setParent(dockLog);
    dockLog->setWidget(winLog);
}

void MainWindow::initStatusBar()
{
    lblBoxeeAddr = new QLabel("  Address 255.255.255.255  ");
    lblBoxeeAddr->setMinimumSize(lblBoxeeAddr->sizeHint());
    lblBoxeeAddr->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(lblBoxeeAddr);

    lblBoxeePort = new QLabel("  Port 65545  ");
    lblBoxeePort->setMinimumSize(lblBoxeePort->sizeHint());
    lblBoxeePort->setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(lblBoxeePort);

    lblBoxeeState = new QLabel(" ");
    lblBoxeeState->setMinimumSize(lblBoxeeState->sizeHint());
    lblBoxeeState->setAlignment(Qt::AlignLeft);
    lblBoxeeState->setIndent(8);
    ui->statusBar->addWidget(lblBoxeeState, 1);
}

void MainWindow::updateStatusBar()
{
    lblBoxeeState->setText(core::Boxee::instance().stateAsString());

    Boxee::State curState = Boxee::instance().state();
    if (curState != Boxee::State::OFF && curState != Boxee::State::SHUTTING_DOWN
        && curState != Boxee::State::BOOTING) {
        lblBoxeeAddr->setText(tr("Address: %1").arg(Boxee::instance().boxeeAddress()));
        lblBoxeePort->setText(tr("Port: %1").arg(Boxee::instance().httpPort()));
    } else {
        lblBoxeeAddr->setText("");
        lblBoxeePort->setText("");
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("cyncrun", "BoxeeSimulator");

    Boxee::instance().setHttpPort(static_cast<uint16_t>(settings.value("httpPort", 8080).toInt()));
    Boxee::instance().setPassword(settings.value("password", "").toString());
    Boxee::instance().setShowBootVideo(settings.value("showBootVideo", true).toBool());
    restoreGeometry(settings.value("mainWinGeometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("cyncrun", "BoxeeSimulator");

    settings.setValue("httpPort", Boxee::instance().httpPort());
    settings.setValue("password", Boxee::instance().password());
    settings.setValue("showBootVideo", Boxee::instance().showBootVideo());
    settings.setValue("mainWinGeometry", saveGeometry());
}

void MainWindow::onPowerOnOff()
{
    if (Boxee::instance().state() == Boxee::State::OFF) {
        Boxee::instance().powerOn();
    } else {
        Boxee::instance().powerOff();
    }
}
