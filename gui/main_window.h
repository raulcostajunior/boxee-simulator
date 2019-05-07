#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QTableView>

namespace Ui {
class MainWindow;
}

namespace gui {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QLabel *lblLogEntries;
    QLabel *lblBoxeeState;

    QDockWidget *dockLog;
    QMainWindow *winLog;
    QTableView *tblLog;
    QToolBar *toolbLog;

    void initActions();
    void initStatusBar();
    void initLogPanel();
};

} // namespace gui

#endif // MAIN_WINDOW_H
