#ifndef PSOCKET_H
#define PSOCKET_H

#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include <QEvent>


class SendLinkEvent;


class pSocket : public QObject
{
    Q_OBJECT
public:
    explicit pSocket(QTcpSocket *socket, QThread* thread);
    ~pSocket();
    
signals:
    void saveFile(const QByteArray& data, const QString& type);
    
public slots:
    void onDataReceived();
    void sendLink(const QString& link);

private:
    QByteArray  _buffer;
    QTcpSocket  *_socket;
    int         _packetSize;
    QString     _fileType;
    QString     _protoVersion;

private:
    QString randName(int count);
    void customEvent(QEvent*);
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
#endif // PSOCKET_H
