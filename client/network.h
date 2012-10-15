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
    explicit Network(QSettings *settings, QObject *parent = 0);
    void uploadFile(const QString &fileName, const QString &type);
    void upload(const QByteArray &data, const QString &type);
signals:
    void linkReceived(const QString &link);
private slots:
    void onDataReceived();
    void lookedUp(const QHostInfo &host);
private:
    QByteArray readFile(const QString& fileName);
private:
    QSettings *_settings;
    bool _ready;
    QTcpSocket _socket;
    QHostAddress _serverAddr;
};
