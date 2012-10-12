#include "logger.h"

QIODevice* Logger::_io = 0;
FILE* Logger::_f = 0;
QString Logger::_path = QString();

bool Logger::configure(const QString &path)
{
//    _path = path;
    _f = fopen(path.toLocal8Bit(), "a");
    return _f;
}

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

//    fprintf(stderr, "%s", msg);

    fflush(_f);
}
