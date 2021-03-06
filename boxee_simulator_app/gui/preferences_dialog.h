#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

namespace gui {

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override;

public slots:
    void accept() override;

private:
    Ui::PreferencesDialog *ui;
};

} // namespace gui

#endif // PREFERENCES_DIALOG_H
