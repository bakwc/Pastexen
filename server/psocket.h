#ifndef PSOCKET_H
#define PSOCKET_H

#include <QTcpSocket>
#include <QByteArray>


class pSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit pSocket(QThread* thread, QObject *parent = 0);
    
signals:
    
public slots:
    void onDataReceived();
    void onDisconnected();

private:
    QByteArray _buffer;
};

#endif // PSOCKET_H
