#include "pserver.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QStringList>
#include <QHostAddress>
#include "pthreadpool.h"
#include "psocket.h"
#include "psaver.h"
#include "logger.h"
#include <QDateTime>

pServer::pServer(QObject *parent):
    QObject(parent)
{
    connect(&_server, SIGNAL(newConnection()), this, SLOT(onConnection()));
    _server.listen(Settings::host(), Settings::port());

    qDebug() << "\n\nServer started. Time:" << QDateTime::currentDateTime().toString();
#ifdef FUNC_DEBUG
    qWarning() << "FuncDebug activated";
#endif
    startTimer(1000 * 60 * 60 * 60 * 24);
}

void pServer::onConnection()
{
    auto socket = _server.nextPendingConnection();

    auto addr = socket->localAddress();
    auto it = _limits.find(addr);
    if (it == _limits.end()) {
        it = _limits.insert(addr, 0);
    } else {
        if (it.value() > 50*1024*1024) {
            socket->disconnectFromHost();
            socket->deleteLater();
            return;
        }
    }
    #ifdef FUNC_DEBUG
    qDebug() << "Limit: " << it.value();
    #endif
    new pSocket(socket, pThreadPool::getNextThread(), it.value());
}

void pServer::timerEvent(QTimerEvent *)
{
    _limits.clear();
}
