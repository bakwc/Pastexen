#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QSslError>
#include <QSettings>
#include "defines.h"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QSettings *settings, QObject *parent = 0);
    void upload(const QString &fileName);
signals:
    void linkReceived(const QString &link);
private slots:
    void onDataReceived();
private:
    QByteArray readFile(const QString& fileName);
private:
    QSettings *_settings;
    bool _ready;
    QTcpSocket _socket;
};

#endif // NETWORK_H
