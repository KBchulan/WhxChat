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

    //  when push the sure btn, it will send a http request
    void on_sure_btn_clicked();

private:
    // show some message(err is red, else green)
    void showTip(QString str, bool b_ok);

    // handle httpRequest, in other words, it will init the _handlers
    void initHttpHandlers();

    // check all edit isValid
    bool CheckUserValid();
    bool CheckEmailValid();
    bool CheckPasswordValid();
    bool CheckConfWordValid();
    bool CheckVarifyCodeValid();

    // add to tip map
    void AddTipErr(TipErr te, QString tips);

    // remove from tip map
    void DelTipErr(TipErr te);

private:
    Ui::RegisterDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QMap<ReqId, std::function<void(const QJsonObject &)>> _handlers;
};

#endif // REGISTERDIALOG_H
