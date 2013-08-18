#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T16:12:12
#
#-------------------------------------------------

CONFIG   -= console
QT       += core gui network gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = pastexen
win32 {
    TARGET = PasteXen
    RC_FILE = PasteXen.rc
}
TEMPLATE = app

CODEC = UTF-8

TRANSLATIONS = pastexen_ru.ts

SOURCES += main.cpp \
    application.cpp \
    configwidget.cpp \
    network.cpp \
    imageselectwidget.cpp \
    languageselectdialog.cpp \
    scanhotkeydialog.cpp \
    ../utils/usettings.cpp \
    ../utils/ufullscreenwrapper.cpp \
    ../utils/uexception.cpp \
    ../utils/ucolonsep.cpp \
    ../utils/ucast.cpp \
    ../utils/ukeysequence.cpp \
    ../utils/uglobalhotkeys.cpp \
    traywindow.cpp

HEADERS  += \
    application.h \
    configwidget.h \
    network.h \
    defines.h \
    imageselectwidget.h \
    utils.h \
    languageselectdialog.h \
    scanhotkeydialog.h \
    ../utils/usettings.h \
    ../utils/ufullscreenwrapper.h \
    ../utils/uexception.h \
    ../utils/ucolonsep.h \
    ../utils/ucast.h \
    ../utils/ukeysequence.h \
    ../utils/uglobalhotkeys.h \
    ../utils/udebug.h \
    traywindow.h

RESOURCES += \
    resources.qrc

FORMS += \
    config.ui \
    languageselectdialog.ui \
    scanHotkeysDialog.ui

INSTALLS += target
isEmpty(PREFIX) {
 PREFIX = /usr/local/bin
}
target.path=$$PREFIX/

QMAKE_CXXFLAGS += -std=c++0x
unix {
    LIBS += -lxcb-keysyms -lX11 -lXtst -lxcb
}
