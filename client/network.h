#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QSslError>
#include <QSettings>
#include <QHostAddress>
#include <QHostInfo>
#include "defines.h"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(const QString& hostName, quint16 port, QObject *parent = 0);
    void uploadFile(const QString &fileName, const QString &type);
    void upload(const QByteArray &data, const QString &type);

    const QTcpSocket& socket() const {
        return _socket;
    }

signals:
    void linkReceived(const QString &link);
    void trayMessage(const QString &caption, const QString &text);
    void ready(); // dns was resolve
private slots:
    void onDataReceived();
    void lookedUp(const QHostInfo &host);
private:
    QByteArray readFile(const QString& fileName);
    void timerEvent(QTimerEvent *);
private:
    QString _hostName;
    quint16 _port;
    bool _ready;
    QTcpSocket _socket;
    QHostAddress _serverAddr;
    int _lookupId;
};
