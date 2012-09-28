#include "pserver.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QStringList>
#include <QHostAddress>
#include "psocket.h"

pServer::pServer(QObject *parent) :
    QObject(parent)
{
    connect(&server, SIGNAL(newConnection()), this, SLOT(onConnection()));

    server.listen(QHostAddress::LocalHost, 9876);
}

void pServer::onConnection()
{
    pSocket *socket = new pSocket(server.nextPendingConnection(), this->thread());

//    pSocket *pS = new pSocket(this->thread());

    qDebug("new connection");
}
