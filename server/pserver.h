#ifndef PSERVER_H
#define PSERVER_H

#include <QObject>
#include <QTcpServer>
#include "psetting.h"

class pThreadPool;

class pServer : public QObject
{
    Q_OBJECT
public:
    explicit pServer(QObject *parent = 0);
    
signals:
    
public slots:
    void onConnection();

private:
    QTcpServer  server;
};

#endif // PSERVER_H
