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

void Logger::logger(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_UNUSED(context);
    switch (type) {
    case QtDebugMsg: {
        fprintf(_f, "%s", msg.toLocal8Bit().data());
        break;
    }
    case QtWarningMsg: {
        fprintf(_f, "Warning: %s", msg.toLocal8Bit().data());
        break;
    }
    case QtCriticalMsg: {
        fprintf(_f, "Critical: %s", msg.toLocal8Bit().data());
        break;
    }
    case QtFatalMsg : {
        fprintf(_f, "Fatal: %s", msg.toLocal8Bit().data());
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
