#-------------------------------------------------
#
# Project created by QtCreator 2013-02-12T09:18:44
#
#-------------------------------------------------

QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thmb-demon

TEMPLATE = app


SOURCES += main.cpp \
    ../server/redisclient.cpp \
    application.cpp

HEADERS += \
    ../server/redisclient.h \
    application.h
