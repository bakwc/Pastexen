#include <QCoreApplication>
#include <QSettings>
#include "pserver.h"
#include "pthreadpool.h"
#include "psetting.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    qsrand(time(NULL));
    QCoreApplication app(argc, argv);

    pSetting setting("../serverSettings.ini");

    pThreadPool::instanse(pSetting::threads());
    pServer     server;

    app.exec();
}

