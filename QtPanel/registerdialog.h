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

private slots:
    void on_get_code_clicked();

private:
    void showTip(QString str, bool b_ok);

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
