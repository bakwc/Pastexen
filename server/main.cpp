#include "application.h"
#include <QSettings>
#include <QTime>
#include "pserver.h"
#include "pthreadpool.h"
#include "psetting.h"
#include "psaver.h"
#include "logger.h"
#include <QFile>

#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

int main(int argc, char** argv)
{
    Application app(argc, argv);
    if (!app.parseArgs()) {
        qDebug() << "Wrong usage!";
        return 42;
    }

    #ifdef Q_OS_LINUX
//    int d = daemon(0, 0);
//    Q_UNUSED(d);
    #endif

    QString pathToConfig = app.config();

    if (app.logFile().size()) {
        if (Logger::configure(app.logFile()))
            qInstallMessageHandler(Logger::logger);
        else
            qDebug() << "Log file not been set";
    }

    Settings setting(pathToConfig);
    pSaver saver;
    pThreadPool pool(Settings::threads());

    pServer server;

    app.exec();
}
