#pragma once

#include <QRunnable>
#include <QString>
#include <QByteArray>

#include "qasyncqueue.h"
#include "psetting.h"

struct Client;
struct LinkReturnTask;

typedef QAsyncQueue<Client*> ClientQueue;

class SaveProcess : public QRunnable
{
public:
    SaveProcess(ClientQueue &saveQueue, ClientQueue &returnQueue);
    void run();
private:
    QString getFileName();
private:
    ClientQueue &_saveQueue;
    ClientQueue &_returnQueue;
};

