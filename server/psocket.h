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
    void saveFile(const QByteArray& data, const QString& type, const QString& filename);
    
public slots:
    void onDataReceived();
    void sendLink(const QString& link);

private:
    QByteArray  _buffer;
    QTcpSocket  *_socket;
    quint32     _packetSize;
    QString     _fileType;

private:
    QString randName(int count);
};

#endif // PSOCKET_H
