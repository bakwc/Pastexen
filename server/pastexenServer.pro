QT += network
QT -= gui

SOURCES += \
    main.cpp \
    pserver.cpp \
    psocket.cpp

HEADERS += \
    pserver.h \
    psocket.h

QMAKE_CXXFLAGS += -std=c++0x
