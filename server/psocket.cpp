#include "psocket.h"
#include <QStringList>


void saveToFile(const QByteArray& data, const QString& type) {
    qDebug() << "File type:" << type << "Data:" << data;
}

QString getValue(const QString& data, const QString& key)
{
    int start = data.indexOf(key);

    QString res = data.mid(start + key.length() + 1);
    res = res.left(res.indexOf("\n"));

    return res;
}

pSocket::pSocket(QTcpSocket *socket, QThread *thread) :
    QObject(0), _socket(socket), _packetSize(0)
{
    connect(_socket, SIGNAL(readyRead()), SLOT(onDataReceived()));
    connect(_socket, SIGNAL(disconnected()), SLOT(deleteLater()));

    //    moveToThread(thread);
}

pSocket::~pSocket()
{
    delete _socket;
}

void pSocket::onDataReceived()
{
    qDebug() << Q_FUNC_INFO;
    auto data = _socket->readAll();
    if (_packetSize == 0) {
        int n = data.indexOf("\n\n");
        auto header = data.left(n);
        QString content = data.mid(n+2);
        _buffer += content;
        _packetSize = getValue(header, "size").toInt();
        _fileType = getValue(header, "type");
    } else {
        _buffer += data;
    }

    qDebug() << "buffer size" << _buffer.size() << _packetSize;
    if (_buffer.size() == _packetSize) {
        saveToFile(_buffer, _fileType);
    }
}

void pSocket::onDisconnected()
{
    QString str = _buffer;




    deleteLater();
}
