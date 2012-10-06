#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QList>
#include <QString>

#include "serverprocess.h"
#include "utils.h"

ServerProcess::ServerProcess(FileSaveQueue &saveQueue, LinkReturnQueue &returnQueue)
    : _saveQueue(saveQueue)
    , _returnQueue(returnQueue)
{
}

void ServerProcess::run()
{
    QTcpServer server;
    QList<Client> clients;
    server.listen(QHostAddress("0.0.0.0"), 9876);
    qDebug() << "Server started at 0.0.0.0:9876";

    while (true) {

        ///          New connection
        server.waitForNewConnection(0);
        QTcpSocket *socket = server.nextPendingConnection();
        if (socket != 0) {
            clients.push_back(Client(socket));
        }

        ///          Processing clients
        for (auto i = clients.begin(); i != clients.end(); i++) {
            i->_socket->waitForReadyRead(0);
            if (i->_socket->bytesAvailable() > 0 && i->_writeEnabled) { // Reading data
                i->_buffer += i->_socket->readAll();

                if (i->_packetSize == 0) {
                    int n = i->_buffer.indexOf("\n\n");
                    if (n == -1) {
                        continue;
                    }
                    QByteArray header = i->_buffer.left(n);
                    QByteArray content = i->_buffer.mid(n+2);
                    i->_buffer = content;
                    i->_packetSize = getValue(header, "size").toInt();
                    i->_fileType = getValue(header, "type");
                }

                if (i->_buffer.size() == i->_packetSize) {
                    qDebug() << "Received a file of type" << i->_fileType;
                    i->_writeEnabled = false;
                    _saveQueue.push(&(*i));
                    i->_packetSize = 0;
                }
            }
        }

        //TODO: link response queue

        Thread::msleep(1000);
    }
}
