#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QIODevice>

class Logger
{

public:
    static void configure(QIODevice* io) { _io = io; }
    static QDebug log() { return QDebug(_io); }

private:
    static QIODevice* _io;
};

QIODevice* Logger::_io = 0;

#endif // LOGGER_H
