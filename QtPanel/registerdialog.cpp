#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    // edit the password and confirm style
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    //
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}
