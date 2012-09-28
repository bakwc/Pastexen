#include "pserver.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QStringList>
#include "psocket.h"

pServer::pServer(QObject *parent) :
    QObject(parent)
{
    connect(&server, SIGNAL(newConnection()), this, SLOT(onConnection()));
}

void pServer::onConnection()
{
    QTcpSocket *socket = server.nextPendingConnection();

    pSocket *pS = new pSocket(this->thread());
}
