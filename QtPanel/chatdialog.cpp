#include "clickedbtn.h"
#include "chatdialog.h"
#include "ui_chatdialog.h"

#include <QDebug>
#include <QAction>
#include <QPainter>

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    // 初始化所有ui
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->setMaxLength(15);

    // search_edit
    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/resources/Chat/mid/search.png"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    // 清除动作和图标
    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/resources/Chat/mid/close_transparent.png"));
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);

    connect(ui->search_edit, &QLineEdit::textChanged, [clearAction](const QString &text) ->void
    {
        if(!text.isEmpty())
            clearAction->setIcon(QIcon(":/resources/Chat/mid/close_search.png"));
        else
            clearAction->setIcon(QIcon(":/resources/Chat/mid/close_transparent.png"));
    });

    connect(clearAction, &QAction::triggered, [this, clearAction]() -> void
    {
         ui->search_edit->clear();
         clearAction->setIcon(QIcon(":/resources/Chat/mid/close_transparent.png"));
         ui->search_edit->clearFocus();
    });

    // 初始化头像
    initHead();
}

ChatDialog::~ChatDialog()
{
    qDebug() << "chatDialog destructed!";
    delete ui;
}

void ChatDialog::initHead()
{
    QPixmap pixmap(":/resources/Chat/left/head.jpg");

    // 纵横比缩放
    pixmap = pixmap.scaled(ui->side_head_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->side_head_label->setPixmap(pixmap);
}
