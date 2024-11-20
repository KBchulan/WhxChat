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

#include "global.h"

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
    // get_code button clicked's callback
    void on_get_code_clicked();

    // when httpManager's register module finished, then the registerDialog will working
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

private:
    // show some message(err is red, else green)
    void showTip(QString str, bool b_ok);

    // handle httpRequest, in other words, it will init the _handlers
    void initHttpHandlers();

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject &)>> _handlers;
};

#endif // REGISTERDIALOG_H
