#include "application.h"
#include <QSettings>
#include <QTime>
#include "pserver.h"
#include "pthreadpool.h"
#include "psetting.h"
#include "psaver.h"
#include "logger.h"
#include <QFile>

int main(int argc, char** argv)
{
    qsrand(QTime::currentTime().msec());
    Application app(argc, argv);
    if (!app.parseArgs()) {
        qDebug() << "Wrong usage!";
        return 42;
    }

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
