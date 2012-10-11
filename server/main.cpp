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
    Application app(argc, argv);
    if (!app.parseArgs())
        return 0;

    QString pathToConfig = app.config();
    QFile file;
    file.setFileName(app.logFile());
    file.open(QIODevice::Append);

    Logger::configure(&file);

    Settings    setting(pathToConfig);
    pSaver      saver;
    pThreadPool pool(Settings::threads());

    pServer     server;

    app.exec();
}
