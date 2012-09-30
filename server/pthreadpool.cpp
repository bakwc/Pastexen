#include "pthreadpool.h"
#include <QtAlgorithms>

pThreadPool* pThreadPool::_inst = 0;
int pThreadPool::index = 0;
QVector<QThread*> pThreadPool::_threads = QVector<QThread*>();

pThreadPool::pThreadPool(int tCount, QObject *parent) :
    QObject(parent)
{
    while(tCount--) {
        QThread *thread = new QThread;
        thread->start();
        _threads.push_back(thread);
    }
}

QThread *pThreadPool::getThread()
{
    if (index == _threads.size())
        index = 0;

    return _threads[index++];
}

pThreadPool *pThreadPool::instanse(int threadCount)
{
    if (_inst == 0)
        _inst = new pThreadPool(threadCount);

    return _inst;
}


