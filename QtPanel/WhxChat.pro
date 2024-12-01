QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatdialog.cpp \
    chatuserlist.cpp \
    chatuserwidget.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    customizeedit.cpp \
    global.cpp \
    httpmanager.cpp \
    listitembase.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    particleeffect.cpp \
    registerdialog.cpp \
    resetdialog.cpp \
    tcpmanager.cpp \
    timerbtn.cpp \
    usermanager.cpp \
    backgrounddialog.cpp

HEADERS += \
    chatdialog.h \
    chatuserlist.h \
    chatuserwidget.h \
    clickedbtn.h \
    clickedlabel.h \
    customizeedit.h \
    global.h \
    httpmanager.h \
    listitembase.h \
    logindialog.h \
    mainwindow.h \
    particleeffect.h \
    registerdialog.h \
    resetdialog.h \
    singleton.h \
    tcpmanager.h \
    timerbtn.h \
    usermanager.h \
    backgrounddialog.h

FORMS += \
    chatdialog.ui \
    chatuserwidget.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    resetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
