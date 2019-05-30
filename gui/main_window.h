#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

class QDockWidget;
class QLabel;
class QTableView;
class QToolBar;

#include <QMainWindow>
#include <QObject>

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

private slots:

    void onPowerOnOff();

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

    void connectBoxeeSignals();

    void updateActions();
    void updateStatusBar();
};

} // namespace gui

#endif // MAIN_WINDOW_H
