#include <QDebug>
#include <QFile>

#include "utils.h"
#include "saveprocess.h"
#include "serverprocess.h"

SaveProcess::SaveProcess(ClientQueue &saveQueue, ClientQueue &returnQueue)
    : _saveQueue(saveQueue)
    , _returnQueue(returnQueue)
{
}

void SaveProcess::run()
{
    while (true) {
        while (!_saveQueue.isEmpty()) {
            Client *client = _saveQueue.pull();

            QFile file;
            QString fileName = getFileName() + "." + client->_fileType;
            QString typeFolder(Settings::types()[client->_fileType]);
            const QString path = typeFolder + fileName;
            file.setFileName(path);

            if (!file.open(QIODevice::WriteOnly)) {
                qDebug() << "Cannot create file:" << file.fileName();
                return;
            }

            file.write(client->_buffer);
            file.close();

            qDebug() << "File saved:" << file.fileName();

            QString url = Settings::prefixes()[client->_fileType] + fileName;
            client->_returnLink = url;
            _returnQueue.push(client);
        }
        Thread::msleep(100);
    }
}

QString SaveProcess::getFileName()
{
    QString str;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < Settings::fileNameLenght(); ++i) {
        str += alphanum[qrand() % (sizeof(alphanum) - 1)];
    }
    return str;
}
