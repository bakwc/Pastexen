#include <QtCore>
#include <QDebug>
#include <QQueue>
#include <QThreadPool>
#include <QMutex>
#include <QTime>
#include <ctime>

#include "utils.h"
#include "serverprocess.h"
#include "saveprocess.h"
#include "psetting.h"

void printUsage()
{
    qDebug() << "Usage: speedserver settings_file.ini";
    exit(1);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (argc != 2) {
        printUsage();
    }
    QString settingsFilename = argv[1];

    ClientQueue saveQueue;
    ClientQueue returnQueue;

    Settings    settings(settingsFilename);
    ServerProcess serverProcess(saveQueue, returnQueue);
    SaveProcess saveProcess(saveQueue, returnQueue);
    QThreadPool serversPool;

    serversPool.start(&serverProcess);
    serversPool.start(&saveProcess);

    while (true) {
        Thread::msleep(10000);
    }
    
    return 0;
}
