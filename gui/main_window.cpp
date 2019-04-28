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
    initToolbar();
    initStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initIconsTheme()
{
    // TODO: add dark or light theme depending on the current shell mode
    QIcon::setThemeName("dark");
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
}

void MainWindow::initActions()
{
    ui->actionQuit->setStatusTip(tr("Quit Boxee Simulator"));
    connect(ui->actionQuit, &QAction::triggered, [this]() { this->close(); });

    ui->actionAbout_Qt->setStatusTip(tr("Display information about Qt version"));
    connect(ui->actionAbout_Qt, &QAction::triggered, []() { qApp->aboutQt(); });

    ui->actionPower_On_Off->setIcon(QIcon::fromTheme("system-shutdown"));
}

void MainWindow::initToolbar()
{
    ui->mainToolBar->addAction(ui->actionPower_On_Off);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionSave_As);
    ui->mainToolBar->addAction(ui->actionClear);
}

void MainWindow::initStatusBar()
{
    lblBoxeeState = new QLabel("OFF");
    lblLogEntries = new QLabel("No data log entries");

    ui->statusBar->addPermanentWidget(lblLogEntries);
    ui->statusBar->addPermanentWidget(lblBoxeeState);
}
