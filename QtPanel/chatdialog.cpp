#include "clickedbtn.h"
#include "chatdialog.h"
#include "loadingdialog.h"
#include "ui_chatdialog.h"

#include <QDebug>
#include <QAction>
#include <QPainter>
#include <QRandomGenerator>

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog),
    _b_loading(false),
    _mode(ChatUIMode::ChatMode),
    _state(ChatUIMode::ChatMode)
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

    // 清除搜索框内容，同时关闭搜索界面
    connect(clearAction, &QAction::triggered, [this, clearAction]() -> void
    {
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/resources/Chat/mid/close_transparent.png"));
        ui->search_edit->clearFocus();
        ShowSearch(false);
    });

    // 接收chatUserList滚动到底部的信号
    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);

    // 隐藏搜索界面
    ShowSearch(false);

    // 初始化头像
    initHead();

    // 添加聊天用户列表
    addChatUserList();
}

ChatDialog::~ChatDialog()
{
    qDebug() << "chatDialog destructed!";
    delete ui;
}

std::vector<QString> strs =
{
    "hello world !",
    "nice to meet u",
    "New year, new life",
    "You have to love yourself",
    "My love is written in the wind ever since the whole world is you"
};

std::vector<QString> heads = 
{
    ":/resources/Chat/mid/4.png",
    ":/resources/Chat/mid/7.jpg"
};

std::vector<QString> names = 
{
    "八奈见杏菜",
    "宫城志绪理",
    "温水佳树",
    "小鞠知花",
    "白玉璃子",
    "志喜屋梦子",
    "马剃天爱星",
    "朝云千早"
};

void ChatDialog::addChatUserList()
{
    for(int i = 0; i < 12; i++)
    {
        int random = QRandomGenerator::global()->bounded(0, names.size());

        int str_i = random % strs.size();
        int name_i = random % names.size();
        int head_i = random % heads.size();

        auto *chat_user_widget = new ChatUserWidget(this);
        chat_user_widget->SetInfo(strs[str_i], names[name_i], heads[head_i]);

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(chat_user_widget->sizeHint());

        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_widget);
    }
}

void ChatDialog::initHead()
{
    QPixmap pixmap(":/resources/Chat/left/head.jpg");

    // 纵横比缩放
    pixmap = pixmap.scaled(ui->side_head_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->side_head_label->setPixmap(pixmap);
}

void ChatDialog::ShowSearch(bool bSearch)
{
    if(bSearch)
    {
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }
    else if(_state == ChatUIMode::ChatMode)
    {
        ui->search_list->hide();
        ui->con_user_list->hide();
        ui->chat_user_list->show();
        _mode = ChatUIMode::ChatMode;
    }
    else if(_state == ChatUIMode::ContactMode)
    {
        ui->search_list->hide();
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading)
        return;

    _b_loading = true;

    LoadingDialog *loadingDialog = new LoadingDialog(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    addChatUserList();
    loadingDialog->deleteLater();

    _b_loading = false;
}
