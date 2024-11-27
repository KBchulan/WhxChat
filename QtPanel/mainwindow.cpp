#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // design ico
    setWindowIcon(QIcon(":/resources/icos/MainWindow.ico"));

    // create LoginDialog
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // show current dialog(default widget is _login_dlg)
    setCentralWidget(_login_dlg);

    // create and register message link
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    // create register
    _reg_dlg = new RegisterDialog(this);

    // set dialogs's style
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(_reg_dlg, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::SlotSwitchLogin()
{
    // create LoginDialog
    _login_dlg = new LoginDialog(this);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // show current dialog(default widget is _login_dlg)
    setCentralWidget(_login_dlg);

    _reg_dlg->hide();
    _login_dlg->show();

    // create and register message link
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

