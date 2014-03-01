#pragma once

#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include <QEvent>
#include <QAtomicInt>

class SendLinkEvent;


class pSocket : public QObject
{
    Q_OBJECT
public:
    explicit pSocket(QTcpSocket *socket, QThread* thread, long long int& limit);
    ~pSocket();
    
signals:
    void saveFile(const QByteArray& data, const QString& type, const QString& uuid);
    
public slots:
    void onDataReceived();
    void sendLink(const QString& link);
private:
    QString randName(int count);
    void customEvent(QEvent*);

private:
    QByteArray  _buffer;
    QTcpSocket  *_socket;
    int         _packetSize;
    QString     _fileType;
    QString     _uuid;
    QString     _protoVersion;
    long long int  &_limit;
};


class SendLinkEvent : public QEvent
{
public:
    static const QEvent::Type TYPE = QEvent::Type(User + 1);

    SendLinkEvent(const QString& l) :
        QEvent(TYPE), _link(l) {}

    const QString& link() { return _link; }
private:
    QString _link;
};

