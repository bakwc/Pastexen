#include "pserver.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QStringList>
#include <QHostAddress>
#include "pthreadpool.h"
#include "psocket.h"
#include "psaver.h"

pServer::pServer(QObject *parent):
    QObject(parent)
{
    connect(&server, SIGNAL(newConnection()), this, SLOT(onConnection()));
    server.listen(Settings::host(), Settings::port());
    qDebug("Server started");
}

void pServer::onConnection()
{
    new pSocket(server.nextPendingConnection(), pThreadPool::getNextThread());
}
