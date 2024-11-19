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
    _login_dlg = new LoginDialog();
    setCentralWidget(_login_dlg);
    _login_dlg->show();

    // create RegisterDialog
    _reg_dlg = new RegisterDialog();

    // create and register message link
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

MainWindow::~MainWindow()
{
    delete ui;

    if(_login_dlg)
    {
        delete _login_dlg;
        _login_dlg = nullptr;
    }

    if(_reg_dlg)
    {
        delete _reg_dlg;
        _reg_dlg = nullptr;
    }
}

