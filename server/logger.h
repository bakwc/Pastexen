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
    static void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void log(const QString &ipAddr, int limit);

private:
    static QIODevice* _io;
    static FILE* _f;
    static QString _path;
};
