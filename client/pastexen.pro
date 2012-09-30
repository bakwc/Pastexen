#-------------------------------------------------
#
# Project created by QtCreator 2012-09-06T16:12:12
#
#-------------------------------------------------

QT       += core gui network testlib

CONFIG  += qxt
CONFIG  -= console
QXT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PasteXen
TEMPLATE = app

SOURCES += main.cpp \
    application.cpp \
    configwidget.cpp \
    network.cpp \
    imageselectwidget.cpp

HEADERS  += \
    application.h \
    configwidget.h \
    network.h \
    defines.h \
    imageselectwidget.h \
    utils.h

RESOURCES += \
    resources.qrc

QMAKE_CXXFLAGS += -std=c++0x

FORMS += \
    config.ui
