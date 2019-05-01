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

void MainWindow::initStatusBar()
{
    lblBoxeeState = new QLabel("Shutting down");
    lblBoxeeState->setMinimumSize(lblBoxeeState->sizeHint());
    ui->statusBar->addPermanentWidget(lblBoxeeState);
}
