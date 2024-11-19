/******************************************************************************
 *
 * @file       registerdialog.h
 * @brief      RegisterDialog
 *
 * @author     KBchulan
 * @date       2024/11/19
 * @history
 *****************************************************************************/
#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui
{
    class RegisterDialog;
}

class RegisterDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
