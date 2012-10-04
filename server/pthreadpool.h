#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H

#include <QObject>
#include <QThread>
#include <QVector>


class pThreadPool : public QObject
{
    Q_OBJECT

public:
    static QThread* getNextThread();
    static pThreadPool* inst() { return _inst; }

    pThreadPool(int tCount, QObject *parent = 0);

signals:
    
public slots:

private:
    QVector<QThread*>    _threads;
    int index;
    static pThreadPool *_inst;

};

#endif // PTHREADPOOL_H
