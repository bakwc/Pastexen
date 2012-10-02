#include "psocket.h"
#include <QStringList>
#include "utils.h"
#include <QTime>
#include "psetting.h"
#include "psaver.h"
#include <QMetaObject>

#ifdef TIME_DEBUG
QTime* debugTime = 0;
#endif

pSocket::pSocket(QTcpSocket *socket, QThread *thread) :
    QObject(0), _socket(socket), _packetSize(0)
{
    connect(_socket, SIGNAL(readyRead()), SLOT(onDataReceived()));
    connect(_socket, SIGNAL(disconnected()), SLOT(deleteLater()));
//    connect(this, SIGNAL(saveFile(QByteArray,QString)), pSaver::inst(), SLOT(save(QByteArray,QString)));

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
    qDebug() << "Disconnect";

//    disconnect();
#ifdef TIME_DEBUG
    qDebug() << 0.001*debugTime->elapsed();
#endif
}


void pSocket::sendLink(const QString& link)
{
    QByteArray arr;
    arr.append("proto=pastexen\n");
    arr.append("version=1.0\n");
    arr.append("url=");
    arr.append(link);
    arr.append("\n\n");
    _socket->write(arr);
}


void pSocket::onDataReceived()
{
    auto data = _socket->readAll();

    if (_packetSize == 0) {
        int n = data.indexOf("\n\n");
        QByteArray header = data.left(n);
//        qDebug() << "Data size: " << data.size();
        auto content = data.mid(n+2);
//        qDebug() << "Content size()" << content.size();
        _buffer = content;
        _packetSize = getValue(header, "size").toInt();
        _fileType = getValue(header, "type");
    } else {
        _buffer += data;
    }

//    qDebug() << "buffer size" << _buffer.size();
//    qDebug() << "packet size" << _packetSize;
    if (_buffer.size() == _packetSize) {
//        const QString filename = saveToFile(_buffer, _fileType, pSetting::types(), pSetting::fileNameLenght());
//        sendLink(pSetting::imageLinkPrefix() + filename);
//        qDebug() << filename;
        _packetSize = 0;

//        emit saveFile(data, _fileType); // I don't know, what faster
        // but i think it
        QMetaObject::invokeMethod(pSaver::inst(), "save",
                                  Q_ARG(QByteArray, data), Q_ARG(QString, _fileType));

    }
}
