#pragma once

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include <QSet>
#include <QHash>

#include "redisclient.h"

class pSaver : public QThread
{
    Q_OBJECT

public:
    explicit pSaver();
    static pSaver* inst() { return pThis; }

signals:
    void fileName(QString str);

public slots:
    void save(const QByteArray& data, const QString& type, const QString& uuid);

private:
    static pSaver  *pThis;
    static QString randName(int count);
    QSet<QString> _files;
    TRedisClient _redis;

    void findFiles();
    QStringList unique(const QStringList& list);
};
