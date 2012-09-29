#include "psocket.h"
#include <QStringList>
#include "utils.h"
#include <QTime>

#ifdef TIME_DEBUG
QTime* debugTime = 0;
#endif

pSocket::pSocket(QTcpSocket *socket, QThread *thread) :
    QObject(0), _socket(socket), _packetSize(0)
{
    connect(_socket, SIGNAL(readyRead()), SLOT(onDataReceived()));
    connect(_socket, SIGNAL(disconnected()), SLOT(deleteLater()));

    _socket->setParent(this);
    moveToThread(thread);

#ifdef TIME_DEBUG
    if (!debugTime) {
        debugTime = new QTime;
        debugTime->start();
    }
#endif
}

pSocket::~pSocket()
{
    delete _socket;

    qDebug() << "Disconnect";
#ifdef TIME_DEBUG
    qDebug() << 0.001*debugTime->elapsed();
#endif
}

void pSocket::onDataReceived()
{
    auto data = _socket->readAll();
    if (_packetSize == 0) {
        int n = data.indexOf("\n\n");
        QByteArray header = data.left(n);
        QString content = data.mid(n+2);
        _buffer += content;
        _packetSize = getValue(header, "size").toInt();
        _fileType = getValue(header, "type");
    } else {
        _buffer += data;
    }

//    qDebug() << "buffer size" << _buffer.size() << _packetSize;
    if (_buffer.size() == _packetSize) {
        saveToFile(_buffer, _fileType);
    }
}
