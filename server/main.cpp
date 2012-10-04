#include <QCoreApplication>
#include <QSettings>
#include "pserver.h"
#include "pthreadpool.h"
#include "psetting.h"
#include "psaver.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    pSetting    setting("../serverSettings.ini");
    pSaver      saver;
    pThreadPool pool(pSetting::threads());

    pServer     server;

    app.exec();
}
