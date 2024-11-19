/******************************************************************************
 *
 * @file       global.h
 * @brief      some headers and global definition
 *
 * @author     KBchulan
 * @date       2024/11/20
 * @history
 *****************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

// qt
#include <QWidget>

// cc
#include <functional>

// personal header
#include "QStyle"

// functional
// repolish err_tip(in register_dialog)'s style(in other words, it will repolish qss)
extern std::function<void(QWidget *)> repolish;

#endif // GLOBAL_H
