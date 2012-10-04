#include "pthreadpool.h"


pThreadPool* pThreadPool::_inst = 0;

pThreadPool::pThreadPool(int tCount, QObject *parent) :
    QObject(parent), index(0)
{
    while(tCount--) {
        QThread *thread = new QThread;
        thread->start();
        _threads.push_back(thread);
    }

    _inst = this;
}

QThread *pThreadPool::getNextThread()
{
    if (inst()->index == inst()->_threads.size())
        inst()->index = 0;

    return inst()->_threads[inst()->index++];
}
