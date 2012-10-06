#include <QtCore>
#include <QDebug>
#include <QQueue>
#include <QThreadPool>
#include <QMutex>

#include "utils.h"
#include "serverprocess.h"
#include "saveprocess.h"

int main(int argc, char *argv[])
{
    FileSaveQueue saveQueue;
    LinkReturnQueue returnQueue;

    ServerProcess serverProcess(saveQueue, returnQueue);
    SaveProcess saveProcess(saveQueue, returnQueue);
    QThreadPool serversPool;

    serversPool.start(&serverProcess);
    serversPool.start(&saveProcess);

    while (true) {
        //qDebug() << "Main thread test";
        Thread::msleep(1000);
    }
    
    return 0;
}
