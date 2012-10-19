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
    #ifdef Q_OS_LINUX
    daemon(0, 0);
    #endif

    Application app(argc, argv);
    if (!app.parseArgs())
        return 0;

    QString pathToConfig = app.config();

    if (app.logFile().size()) {
        if (Logger::configure(app.logFile()))
            qInstallMsgHandler(Logger::logger);
        else
            qDebug() << "Log file not been set";
    }

    Settings setting(pathToConfig);
    pSaver saver;
    pThreadPool pool(Settings::threads());

    pServer server;

    app.exec();
}