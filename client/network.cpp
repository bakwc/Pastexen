#include <QHttpRequestHeader>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QtTest/QTest>
#include <QHostAddress>
#include "network.h"
#include "utils.h"

Network::Network(QSettings *settings, QObject *parent) :
    QObject(parent),
    _settings(settings)
{
    connect(&_socket, SIGNAL(readyRead()), SLOT(onDataReceived()));
}

void Network::upload(const QString &filename)
{
    qDebug() << Q_FUNC_INFO;
    auto data = readFile(filename);

    _socket.connectToHost(QHostAddress::LocalHost, 9876);
    _socket.waitForConnected();

    QByteArray arr;
    arr.append("proto=pastexen\n");
    arr.append("version=1.0\n");
    arr.append("type=png\n");
    arr.append(QString("size=%1\n\n").arg(data.size()));
    arr.append(data);
    _socket.write(arr);
    qDebug() << "Data size: " << arr.size();
    qDebug() << "Upload started!";
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
