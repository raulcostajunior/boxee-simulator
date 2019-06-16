#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

class QCloseEvent;
class QDockWidget;
class QLabel;
class QTableView;
class QToolBar;

class LogView;

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

protected:
    void closeEvent(QCloseEvent *);

private slots:

    void onPowerOnOff();

private:
    Ui::MainWindow *ui;

    QLabel *lblLogEntries;
    QLabel *lblBoxeeState;

    QDockWidget *dockLog;
    QMainWindow *winLog;
    LogView *tblLog;
    QToolBar *toolbLog;

    void initActions();
    void initStatusBar();
    void initLogPanel();

    void connectSignals();

    void updateActions();
    void updateStatusBar();

    void loadSettings();
    void saveSettings();
};

} // namespace gui

#endif // MAIN_WINDOW_H
