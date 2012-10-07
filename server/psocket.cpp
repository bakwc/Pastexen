#include "psocket.h"
#include <QStringList>
#include "utils.h"
#include "psetting.h"
#include "psaver.h"
#include <QMetaObject>

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
    connect(this, SIGNAL(saveFile(QByteArray,QString,QString)), pSaver::inst(), SLOT(save(QByteArray,QString,QString)));

    _socket->setParent(this);
    moveToThread(thread);

    qDebug() << "New connection";

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
    QByteArray arr;
    arr.append("proto=pastexen\n");
    arr.append("version=1.0\n");
    arr.append("url=");
    arr.append(link);
    arr.append("\n\n");
    _socket->write(arr);

    qDebug() << link;
}


void pSocket::onDataReceived()
{
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

    if (_buffer.size() == _packetSize) {
        _packetSize = 0;

        const QString filename = randName(Settings::fileNameLenght()) + '.' + _fileType;
        emit saveFile(data, _fileType, filename);
        sendLink(Settings::prefixes()[_fileType] + filename);

#ifdef TIME_DEBUG
        qDebug() << dTime->elapsed();
#endif
    }
}


QString pSocket::randName(int count)
{
    QString str;

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    qsrand(QTime().msecsTo(QTime::currentTime()));
    for (int i = 0; i < count; ++i) {
        str += alphanum[qrand() % (sizeof(alphanum) - 1)];
    }

    return str;
}
