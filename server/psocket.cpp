#include "psocket.h"
#include <QStringList>
#include "utils.h"
#include "psetting.h"
#include "psaver.h"
#include <QMetaObject>
#include <QCoreApplication>

#ifdef TIME_DEBUG
#include <QTime>
QTime *dTime = 0;
#endif


pSocket::pSocket(QTcpSocket *socket, QThread *thread) :
    QObject(0), _socket(socket), _packetSize(0)
{
    connect(_socket, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
//    connect(_socket, SIGNAL(disconnected()), _socket, SLOT(deleteLater()));
    connect(this, SIGNAL(saveFile(QByteArray,QString)), pSaver::inst(), SLOT(save(QByteArray,QString)));

    _socket->setParent(this);
    moveToThread(thread);

#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO << "New connection" << socket->localAddress();
#endif

#ifdef TIME_DEBUG
    if (!dTime) {
        dTime = new QTime;
        dTime->start();
    }
#endif
}

pSocket::~pSocket()
{
}


void pSocket::sendLink(const QString& link)
{
#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif

    QByteArray arr;
    arr.append("proto=pastexen\n");
    arr.append("version=1.0\n");
    arr.append("url=");
    arr.append(link);
    arr.append("\n\n");
    _socket->write(arr);

#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO << link;
#endif
}


void pSocket::onDataReceived()
{
#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif

    auto data = _socket->readAll();

    if (_packetSize == 0) {
        int n = data.indexOf("\n\n");
        QByteArray header = data.left(n);
        auto content = data.mid(n+2);
        _buffer = content;
        _packetSize = getValue(header, "size").toInt();
        _fileType = getValue(header, "type");
    } else {
        _buffer += data;
    }

#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO << "read";
#endif

    if (_buffer.size() == _packetSize) {

#ifdef FUNC_DEBUG
        qDebug() << Q_FUNC_INFO << "emit";
#endif

        _packetSize = 0;
        emit saveFile(_buffer, _fileType);

#ifdef TIME_DEBUG
        qDebug() << dTime->elapsed();
#endif
    }

#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO << "end";
#endif
}


void pSocket::customEvent(QEvent *ev)
{
#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO;
#endif

    if (ev->type() == SendLinkEvent::TYPE) {
        SendLinkEvent *event = (SendLinkEvent*)ev;

#ifdef FUNC_DEBUG
        qDebug() << Q_FUNC_INFO << event->link();
#endif

        sendLink(event->link());
    }

#ifdef FUNC_DEBUG
    qDebug() << Q_FUNC_INFO << "end";
#endif
}
