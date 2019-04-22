#include "main_window.h"
#include "ui_main_window.h"

#include "dto/net_message.h"

using namespace gui;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
