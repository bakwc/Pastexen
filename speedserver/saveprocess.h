#pragma once

#include <QRunnable>
#include <QString>
#include <QByteArray>

#include "qasyncqueue.h"

struct Client;

struct LinkReturnTask
{
    QString _link;
    int _clientSocketDescriptor;
};

typedef QAsyncQueue<Client*> FileSaveQueue;
typedef QAsyncQueue<LinkReturnTask> LinkReturnQueue;

class SaveProcess : public QRunnable
{
public:
    SaveProcess(FileSaveQueue &saveQueue, LinkReturnQueue &returnQueue);
    void run();
private:
    FileSaveQueue &_saveQueue;
    LinkReturnQueue &_returnQueue;
};

