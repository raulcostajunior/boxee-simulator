#include "gui/main_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gui::MainWindow w;
    w.show();

    return a.exec();
}
