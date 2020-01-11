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

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/roleview.cpp \
    src/lgl.cpp \
    src/sqlite3.c \
    src/dbio.cpp \
    src/dialoglabel.cpp \
    src/getgoodslabel.cpp \
    src/storyscene.cpp \
    src/battlescene.cpp \
    src/role.cpp \
    src/startbutton.cpp \
    src/battleroleselectscene.cpp \
    src/aboutdialog.cpp \
    src/roleinfowidget.cpp \
    src/saveloadwidget.cpp \
    src/battlelabel.cpp \
    src/roleanimation.cpp \
    src/hpslotwidget.cpp \
    src/settingsdialog.cpp \
    src/fadescene.cpp \
    src/menuwidget.cpp \
    src/operationwidget.cpp \
    src/weaponwidget.cpp \
    src/cursorwidget.cpp \
    src/battleaimwidget.cpp \
    src/astar.cpp \
    src/statuswidget.cpp \
    src/tradewidget.cpp \
    src/maparray.cpp \
    src/descriptionwidget.cpp

HEADERS  += src/mainwindow.h \
    src/lgl.h \
    src/roleview.h \
    src/sqlite3.h \
    src/dbio.h \
    src/dialoglabel.h \
    src/getgoodslabel.h \
    src/storyscene.h \
    src/battlescene.h \
    src/role.h \
    src/startbutton.h \
    src/battleroleselectscene.h \
    src/aboutdialog.h \
    src/roleinfowidget.h \
    src/saveloadwidget.h \
    src/battlelabel.h \
    src/roleanimation.h \
    src/hpslotwidget.h \
    src/settingsdialog.h \
    src/fadescene.h \
    src/menuwidget.h \
    src/operationwidget.h \
    src/weaponwidget.h \
    src/cursorwidget.h \
    src/battleaimwidget.h \
    src/astar.h \
    src/statuswidget.h \
    src/tradewidget.h \
    src/maparray.h \
    src/descriptionwidget.h \
    src/weapon.h

FORMS    += ui/mainwindow.ui \
    ui/aboutdialog.ui \
    ui/settingsdialog.ui

TRANSLATIONS += \
    lgl_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
