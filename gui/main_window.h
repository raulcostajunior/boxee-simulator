#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QLabel>
#include <QMainWindow>

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

    void initIconsTheme();
    void initActions();
    void initToolbar();
    void initStatusBar();
};

} // namespace gui

#endif // MAIN_WINDOW_H
