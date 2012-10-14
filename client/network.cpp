#include <QHttpRequestHeader>
#include <QHostInfo>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QTime>
#include "network.h"
#include "utils.h"
#include "application.h"

Network::Network(QSettings *settings, QObject *parent) :
    QObject(parent),
    _settings(settings)
{
    QList<QHostAddress> addrs = QHostInfo::fromName("pastexen.com").addresses();
    if (!addrs.size()) {
        qDebug() << "No internet connection";
    } else {
        _serverAddr = addrs.at(0);
        connect(&_socket, SIGNAL(readyRead()), SLOT(onDataReceived()));
    }
}

void Network::upload(const QByteArray& data, const QString &type)
{
    _socket.connectToHost(_serverAddr, 9876);
    _socket.waitForConnected();

    QByteArray arr;
    arr.append("proto=pastexen\n");
    arr.append("version=1.0\n");
    arr.append("type=" + type + "\n");
    arr.append(QString("size=%1\n\n").arg(data.size()));
    arr.append(data);
    _socket.write(arr);
}

QByteArray Network::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening temporary file!";
        exit(1);
    }
    return file.readAll();
}

void Network::onDataReceived()
{
    const QByteArray arr = _socket.readAll();
    const QString link = getValue(arr, "url");
    emit linkReceived(link);
}
