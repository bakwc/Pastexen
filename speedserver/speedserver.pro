#-------------------------------------------------
#
# Project created by QtCreator 2012-10-06T20:23:46
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = speedserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    serverprocess.cpp \
    saveprocess.cpp \
    utils.cpp

HEADERS += \
    utils.h \
    serverprocess.h \
    saveprocess.h \
    qasyncqueue.h
