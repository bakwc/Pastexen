#include "logger.h"
#include <QDateTime>
#include "pserver.h"

QIODevice* Logger::_io = 0;
FILE* Logger::_f = stderr;
QString Logger::_path = QString();

bool Logger::configure(const QString &path)
{
    _f = fopen(path.toLocal8Bit(), "a");
    return _f;
}

//void Logger::logger(QtMsgType type, const QMessageLogContext& context, const QString& msg)
void Logger::logger(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtDebugMsg: {
        fprintf(_f, "%s", msg);
        break;
    }
    case QtWarningMsg: {
        fprintf(_f, "Warning: %s", msg);
        break;
    }
    case QtCriticalMsg: {
        fprintf(_f, "Critical: %s", msg);
        break;
    }
    case QtFatalMsg : {
        fprintf(_f, "Fatal: %s", msg);
        break;
    }
    }

    fflush(_f);
}

void Logger::log(const QString& ipAddr, int limit)
{
    QDateTime t = QDateTime::currentDateTime();
    fprintf(_f, "%d connected %s %d\n",
            t.toTime_t(), ipAddr.toLocal8Bit().data(), limit);
}
