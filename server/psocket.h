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

private:
    QByteArray  _buffer;
    QTcpSocket  *_socket;
    quint32     _packetSize;
    QString     _fileType;

private:
    void sendLink(const QString& link);
};

#endif // PSOCKET_H
