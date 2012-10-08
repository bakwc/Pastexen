#include <QCoreApplication>
#include <QSettings>
#include <QTime>
#include "pserver.h"
#include "pthreadpool.h"
#include "psetting.h"
#include "psaver.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QString pathToConfig;

    if (argc < 2) {
        qDebug() << "Usage:\n"
                 << "pastexenServer [config.file]\n";
    } else {
        pathToConfig = argv[1];
    }

    Settings    setting(pathToConfig);
    pSaver      saver;
    pThreadPool pool(Settings::threads());

    pServer     server;

    app.exec();
}
