#pragma once

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

