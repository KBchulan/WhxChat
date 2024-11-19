#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // design ico
    setWindowIcon(QIcon(":/resources/icos/MainWindow.ico"));

    // create LoginDialog, RegisterDialog
    _login_dlg = new LoginDialog(this);
    _reg_dlg = new RegisterDialog(this);

    // set dialogs's style
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // create and register message link
    connect(_login_dlg, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);

    // show current dialog(default widget is _login_dlg)
    _reg_dlg->hide();
    setCentralWidget(_login_dlg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(_reg_dlg);
    _login_dlg->hide();
    _reg_dlg->show();
}

