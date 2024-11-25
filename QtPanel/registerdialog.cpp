#include "httpmanager.h"
#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    // edit the password and confirm to password style
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    // set err_tip's property
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);

    // connect to httpManager module finished signals
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    // init the handlers
    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    auto email = ui->email_edit->text();

    // （nums + letters + specials) + '@' + （nums + letters + specials) + '.' + (at least two letter)
    QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    bool match = regex.match(email).hasMatch();

    if(match)
    {
        // The Server send http confirm-code
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpManager::GetInstance()->PostHttpReq(gate_url_prefix + "/get_varifycode", json_obj,
                                                ReqId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
    }
    else
    {
        showTip(tr("The email-address is error!"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("The net request error"), false);
        return;
    }

    // parse json string, res -> QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isEmpty())
    {
        showTip(tr("Json parse failed"), false);
        return;
    }
    if(!jsonDoc.isObject())
    {
        showTip(tr("Json parse error"), false);
        return;
    }

    // .json to jsonObject
    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::showTip(QString str, bool b_ok)
{
    if(b_ok)
    {
        ui->err_tip->setProperty("state", "normal");
    }
    else
    {
        ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void RegisterDialog::initHttpHandlers()
{
    // get_varify_code's handler
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject &jsonObj)
    {
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("Paramers error"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("The varify_code has already sent, please check out it."), true);
        qDebug() << "email is: " << email;
        qDebug() << "error is: " << error;
    });

    // register user's handler
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj)
    {
        int error = jsonObj["error"].toInt();

        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("params error!"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("user register success!"), true);
        qDebug() << "email is: " << email;
    });
}

void RegisterDialog::on_sure_btn_clicked()
{
    if (ui->user_edit->text() == "")
    {
        showTip(tr("用户名不能为空"), false);
        return;
    }

    if (ui->email_edit->text() == "")
    {
        showTip(tr("邮箱不能为空"), false);
        return;
    }

    if (ui->pass_edit->text() == "")
    {
        showTip(tr("密码不能为空"), false);
        return;
    }

    if (ui->confirm_edit->text() == "")
    {
        showTip(tr("确认密码不能为空"), false);
        return;
    }

    if (ui->confirm_edit->text() != ui->pass_edit->text())
    {
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }

    if (ui->varify_edit->text() == "")
    {
        showTip(tr("验证码不能为空"), false);
        return;
    }

    // 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->pass_edit->text();
    json_obj["confirm"] = ui->confirm_edit->text();
    json_obj["varifycode"] = ui->varify_edit->text();

    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                            json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}
