#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H

#include <QObject>
#include <QThread>
#include <QVector>


class pThreadPool : public QObject
{
    Q_OBJECT

public:
    static QThread* getThread();
    static pThreadPool* instanse(int threadCount = QThread::idealThreadCount());

protected:
    pThreadPool(int tCount, QObject *parent = 0);

signals:
    
public slots:

private:
    static QVector<QThread*>    _threads;
    static int index;
    static pThreadPool *_inst;
};

#endif // PTHREADPOOL_H
