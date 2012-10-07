#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QList>
#include <QString>
#include <QTime>

#include "serverprocess.h"
#include "utils.h"

ServerProcess::ServerProcess(ClientQueue &saveQueue, ClientQueue &returnQueue)
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

    QTime timer;
    timer.start();
    int t = 0;
    while (true) {

        ///          New connection
        server.waitForNewConnection(0);
        QTcpSocket *socket = server.nextPendingConnection();

        while (socket != 0) {
            clients.push_back(Client(socket));
            if (t==0) {
                timer.restart();
            }
            socket = server.nextPendingConnection();
        }


        ///         Removing old clients
        for (auto i = clients.begin(); i != clients.end();) {
            if (i->_socket->state() == QAbstractSocket::UnconnectedState) {
                i = clients.erase(i);
                qDebug() << "Client disconnected";
            } else {
                i++;
            }
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
                }
            }
        }

        ///         Returning links to clients
        while (!_returnQueue.isEmpty()) {
            Client *client = _returnQueue.pull();
            qDebug() << "Sending link back:" << client->_returnLink;

            QByteArray arr;
            arr.append("proto=pastexen\n");
            arr.append("version=1.0\n");
            arr.append("url=");
            arr.append(client->_returnLink);
            arr.append("\n\n");
            client->_socket->write(arr);
            client->_socket->disconnectFromHost();
        }
    }
}
