#include <QDebug>

#include "utils.h"
#include "saveprocess.h"
#include "serverprocess.h"

SaveProcess::SaveProcess(FileSaveQueue &saveQueue, LinkReturnQueue &returnQueue)
    : _saveQueue(saveQueue)
    , _returnQueue(returnQueue)
{
}

void SaveProcess::run()
{
    while (true) {
        while (!_saveQueue.isEmpty()) {
            Client *client = _saveQueue.pull();
            qDebug() << "Saving file";
        }
        Thread::msleep(500);
    }
}
