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

    // connect to httpManager module finished signals
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    // init the handlers
    initHttpHandlers();

    // first of all, clear the err_tip.text(), and connect to all condition
    ui->err_tip->clear();

    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this]()
    {
        CheckUserValid();
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]()
    {
        CheckEmailValid();
    });

    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this]()
    {
        CheckPasswordValid();
    });

    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this]()
    {
        CheckConfWordValid();
    });

    connect(ui->varify_edit, &QLineEdit::editingFinished, this, [this]()
    {
        CheckVarifyCodeValid();
    });

    ui->pass_visible->SetState("unvisible", "unvisible_hover", "unvisible_hover", "visible", "visible_hover", "visible_hover");
    ui->confirm_visible->SetState("unvisible", "unvisible_hover", "unvisible_hover", "visible", "visible_hover", "visible_hover");

    connect(ui->pass_visible, &ClickedLabel::sig_clicked, this, [this]()
    {
        auto state = ui->pass_visible->GetCurState();
        if(state == ClickLbState::Normal)
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        else
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
    });

    connect(ui->confirm_visible, &ClickedLabel::sig_clicked, this, [this]()
    {
        auto state = ui->confirm_visible->GetCurState();
        if(state == ClickLbState::Normal)
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        else
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
    });

    _countdown_timer = new QTimer(this);

    connect(_countdown_timer, &QTimer::timeout, [this]()
    {
        if(_countdown == 0)
        {
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回登录界面").arg(_countdown);
        ui->tip_lb->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    qDebug() << "register destruce";
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    auto email = ui->email_edit->text();
    bool valid = CheckEmailValid();

    if(valid)
    {
        // The Server send http confirm-code
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpManager::GetInstance()->PostHttpReq(gate_url_prefix + "/get_varifycode", json_obj,
                                                ReqId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"), false);
        return;
    }

    // parse json string, res -> QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isEmpty())
    {
        showTip(tr("Json解析错误"), false);
        return;
    }
    if(!jsonDoc.isObject())
    {
        showTip(tr("Json解析失败"), false);
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
            showTip(tr("参数错误"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送，请注意查收."), true);
    });

    // register user's handler
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj)
    {
        int error = jsonObj["error"].toInt();

        // 此处可以显示具体原因
        if(error != ErrorCodes::SUCCESS)
        {
            if(error == 1005)
                showTip(tr("用户已存在!"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("注册成功!"), true);
        ChangeTipPage();
    });
}

void RegisterDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty())
    {
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

bool RegisterDialog::CheckUserValid()
{
    if(ui->user_edit->text().trimmed().isEmpty())
    {
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::CheckEmailValid()
{
    auto email = ui->email_edit->text();

    // （nums + letters + specials) + '@' + （nums + letters + specials) + '.' + (at least two letter)
    QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    bool match = regex.match(email).hasMatch();

    if(!match)
    {
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::CheckPasswordValid()
{
    auto password = ui->pass_edit->text();

    // 先检测长度
    if(password.length() < 6 || password.length() > 15)
    {
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度不准确,应在6~15位"));
        return false;
    }

    QRegularExpression regex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)[A-Za-z\\d!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]{6,15}$");
    bool match = regex.match(password).hasMatch();

    if(!match)
    {
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码必须包含大小写字母和数字"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool RegisterDialog::CheckConfWordValid()
{
    auto pass = ui->pass_edit->text();
    auto confirm = ui->confirm_edit->text();

    if(pass != confirm)
    {
        AddTipErr(TipErr::TIP_PWD_CONFIRM, tr("两次密码不同"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_CONFIRM);
    return true;
}

bool RegisterDialog::CheckVarifyCodeValid()
{
    auto varifyCode = ui->varify_edit->text();

    if(varifyCode.length() == 0)
    {
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::on_sure_btn_clicked()
{
    if (!CheckUserValid())
        return;

    if (!CheckEmailValid())
        return;

    if (!CheckPasswordValid())
        return;

    if (!CheckConfWordValid())
        return;

    if (!CheckVarifyCodeValid())
        return;

    // 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->pass_edit->text());
    json_obj["confirm"] = xorString(ui->confirm_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();

    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                            json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

void RegisterDialog::ChangeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    _countdown_timer->start(1000);
}

void RegisterDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

void RegisterDialog::on_cancel_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}
