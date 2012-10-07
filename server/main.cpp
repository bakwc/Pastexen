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

    Settings    setting("../serverSettings.ini");
    pSaver      saver;
    pThreadPool pool(Settings::threads());

    pServer     server;

    app.exec();
}
