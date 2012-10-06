#pragma once

#include <QRunnable>
#include <QtNetwork/QTcpSocket>

#include "saveprocess.h"

struct Client
{
    Client(QTcpSocket *socket)
        : _socket(socket)
        , _packetSize(0)
        , _writeEnabled(true)
    {}
    QTcpSocket* _socket;
    QByteArray _buffer;
    int _packetSize;
    bool _writeEnabled;
    QString _fileType;
};

class ServerProcess : public QRunnable
{
public:
    ServerProcess(FileSaveQueue &saveQueue, LinkReturnQueue &returnQueue);
    void run();
private:
    FileSaveQueue &_saveQueue;
    LinkReturnQueue &_returnQueue;
};
