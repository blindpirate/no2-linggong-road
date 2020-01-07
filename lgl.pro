QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    roleview.cpp \
    lgl.cpp \
    sqlite3.c \
    dbio.cpp \
    dialoglabel.cpp \
    getgoodslabel.cpp \
    storyscene.cpp \
    battlescene.cpp \
    role.cpp \
    startbutton.cpp \
    battleroleselectscene.cpp \
    aboutdialog.cpp \
    roleinfowidget.cpp \
    saveloadwidget.cpp \
    battlelabel.cpp \
    roleanimation.cpp \
    hpslotwidget.cpp \
    settingsdialog.cpp \
    fadescene.cpp \
    menuwidget.cpp \
    operationwidget.cpp \
    weaponwidget.cpp \
    cursorwidget.cpp \
    battleaimwidget.cpp \
    astar.cpp \
    statuswidget.cpp \
    tradewidget.cpp \
    maparray.cpp \
    descriptionwidget.cpp

HEADERS  += mainwindow.h \
    lgl.h \
    roleview.h \
    sqlite3.h \
    dbio.h \
    dialoglabel.h \
    getgoodslabel.h \
    storyscene.h \
    battlescene.h \
    role.h \
    startbutton.h \
    battleroleselectscene.h \
    aboutdialog.h \
    roleinfowidget.h \
    saveloadwidget.h \
    battlelabel.h \
    roleanimation.h \
    hpslotwidget.h \
    settingsdialog.h \
    fadescene.h \
    menuwidget.h \
    operationwidget.h \
    weaponwidget.h \
    cursorwidget.h \
    battleaimwidget.h \
    astar.h \
    statuswidget.h \
    tradewidget.h \
    maparray.h \
    descriptionwidget.h \
    weapon.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    settingsdialog.ui

TRANSLATIONS += \
    lgl_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
