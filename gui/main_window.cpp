#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QTableView>
#include <QToolBar>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initActions()
{
    connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });
    connect(ui->actionAbout_Qt, &QAction::triggered, []() { qApp->aboutQt(); });
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
    lblBoxeeState = new QLabel("Shutting down");
    lblBoxeeState->setMinimumSize(lblBoxeeState->sizeHint());
    ui->statusBar->addPermanentWidget(lblBoxeeState);
}
