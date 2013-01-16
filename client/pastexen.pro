#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T16:12:12
#
#-------------------------------------------------

CONFIG   -= console
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = pastexen
win32 {
    TARGET = PasteXen
    RC_FILE = PasteXen.rc
}
TEMPLATE = app

SOURCES += main.cpp \
    application.cpp \
    configwidget.cpp \
    network.cpp \
    imageselectwidget.cpp \
    languageselectdialog.cpp \
    scanhotkeydialog.cpp

HEADERS  += \
    application.h \
    configwidget.h \
    network.h \
    defines.h \
    imageselectwidget.h \
    utils.h \
    languageselectdialog.h \
    scanhotkeydialog.h

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

