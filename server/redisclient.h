#pragma once

#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QtNetwork>

class TRedisClient: public QObject
{
    Q_OBJECT
public:
    TRedisClient();
    void SetTimeout(int);
    int GetTimeout();
    bool Connect(QString,int);
    int GetErrorCode();
    QString GetErrorString();
    QStringList Query(QString);
    bool IsConnected();
    bool Disconnect();
private:
    QTcpSocket* Socket = new QTcpSocket(this);
    int Timeout = 3000;
    bool SendRequest(QString);
    QStringList ReadResponse();
};
