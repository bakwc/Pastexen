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

    server.listen(pSetting::host(), pSetting::port());
}

void pServer::onConnection()
{
    /*pSocket *socket = */new pSocket(server.nextPendingConnection(), pThreadPool::getThread());
}
