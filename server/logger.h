#pragma once

#include <QObject>
#include <QDebug>
#include <QIODevice>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <fstream>

class Logger : public QObject
{

public:
    static bool configure(const QString& path);
    static void logger(QtMsgType type, const char* msg);

private:
    static QIODevice* _io;
    static FILE* _f;
    static QString _path;
};
