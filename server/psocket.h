#ifndef PSOCKET_H
#define PSOCKET_H

#include <QTcpSocket>
#include <QByteArray>


class pSocket : public QObject
{
    Q_OBJECT
public:
    explicit pSocket(QTcpSocket *socket, QThread* thread);
    ~pSocket();
    
signals:
    
public slots:
    void onDataReceived();
    void onDisconnected();

private:
    QByteArray  _buffer;
    QTcpSocket  *_socket;
    quint32     _packetSize;
    QString     _fileType;
};

#endif // PSOCKET_H
