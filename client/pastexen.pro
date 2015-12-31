#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T16:12:12
#
#-------------------------------------------------

CONFIG   -= console
QT       += core gui network gui-private

CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = pastexen
TEMPLATE = app

QTPLUGIN     += qico

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
    traywindow.cpp \
    screenshoteditwidget.cpp \
    autorunmanager.cpp

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
    traywindow.h \
    screenshoteditwidget.h \
    autorunmanager.h

RESOURCES += \
    resources.qrc

win32:QMAKE_PRE_LINK = "windres --use-temp-file -i ../client/resources.rc -o resources.o --include-dir=./client"

win32:LIBS += "resources.o"

FORMS += \
    config.ui \
    languageselectdialog.ui \
    scanHotkeysDialog.ui \
    screenshoteditwidget.ui

INSTALLS += target
isEmpty(PREFIX) {
 PREFIX = /usr/local/bin
}
target.path=$$PREFIX/

QMAKE_CXXFLAGS += -std=c++0x

#  for static build:
#QMAKE_CXXFLAGS = -std=c++0x -static
#QMAKE_LFLAGS_RELEASE = -static -static-libgcc -static-libstdc++ -s

linux {
    LIBS += -lxcb-keysyms -lX11 -lXtst -lxcb
}

QMAKE_INFO_PLIST=$${PWD}/Info.plist

mac {
    LIBS += -framework Carbon
}

DEFINES += UGLOBALHOTKEY_NOEXPORT

windows {
    DEFINES += WITH_LIBQTSCREEN=1
    include(../LibQtScreen/libqtscreen/libqtscreen.pri)
}
