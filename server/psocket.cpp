#include "psocket.h"

pSocket::pSocket(QThread *thread, QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), SLOT(onDataReceived()));
    connect(this, SIGNAL(disconnected()), SLOT(onDisconnected()));

    moveToThread(thread);

}

void pSocket::onDataReceived()
{
    _buffer += this->readAll();
}

void pSocket::onDisconnected()
{

}
