#include <QCoreApplication>
#include "pserver.h"
#include "pthreadpool.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    pThreadPool::instanse();
    pServer     server;

    app.exec();
}

