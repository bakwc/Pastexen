#include <QCoreApplication>
#include "pserver.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    pServer server;

    app.exec();
}

