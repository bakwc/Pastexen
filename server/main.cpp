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

    if (argc < 2) {
        qDebug() << "Usage:\n"
                 << "pastexenServer \[config.file\]\n";
    }

    Settings    setting(argv[1]);
    pSaver      saver;
    pThreadPool pool(Settings::threads());

    pServer     server;

    app.exec();
}
