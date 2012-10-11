QT += core network
QT -= gui

SOURCES += \
    main.cpp \
    pserver.cpp \
    psocket.cpp \
    pthreadpool.cpp \
    psetting.cpp \
    psaver.cpp \
    application.cpp \
    logger.cpp

HEADERS += \
    pserver.h \
    psocket.h \
    utils.h \
    pthreadpool.h \
    psetting.h \
    psaver.h \
    logger.h \
    application.h

QMAKE_CXXFLAGS += -std=c++0x

DEFINES += \
#            TIME_DEBUG \
#            FUNC_DEBUG

OTHER_FILES +=
