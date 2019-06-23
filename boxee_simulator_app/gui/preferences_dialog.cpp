#include "preferences_dialog.h"

#include "core/boxee.h"
#include "ui_preferences_dialog.h"
#include <cstdint>

using namespace gui;
using namespace core;

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    ui->edtPort->setValue(Boxee::instance().httpPort());
    ui->edtPassword->setText(Boxee::instance().password());
    ui->chkBootVideo->setCheckState(Boxee::instance().showBootVideo() ? Qt::Checked : Qt::Unchecked);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::accept()
{
    Boxee::instance().setHttpPort(static_cast<uint16_t>(ui->edtPort->value()));
    Boxee::instance().setPassword(ui->edtPassword->text());
    Boxee::instance().setShowBootVideo(ui->chkBootVideo->isChecked());

    QDialog::accept();
}
