#pragma once

#include <QRunnable>
#include <QtNetwork/QTcpSocket>

#include "saveprocess.h"
#include "psetting.h"

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
    QString _returnLink;
};

class ServerProcess : public QRunnable
{
public:
    ServerProcess(ClientQueue &saveQueue, ClientQueue &returnQueue);
    void run();
private:
    ClientQueue &_saveQueue;
    ClientQueue &_returnQueue;
};
