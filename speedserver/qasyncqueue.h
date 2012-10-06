#pragma once

#include <QThread>
#include <QQueue>
#include <QMutex>

template<typename T>
class QAsyncQueue
{
public:
    QAsyncQueue(uint max = -1)
    : _max(max)
    {
    }

    ~QAsyncQueue()
    {
        clean();
    }

    uint count()
    {
        _mutex.lock();
        int count = _queue.count();
        _mutex.unlock();
        return count;
    }

    bool isFull()
    {
        if (-1 == _max)
        return false;

        _mutex.lock();
        int count = _queue.count();
        _mutex.unlock();
        return count >= _max;
    }

    bool isEmpty()
    {
        _mutex.lock();
        bool empty = _queue.isEmpty();
        _mutex.unlock();
        return empty;
    }

    void clean()
    {
        _mutex.lock();
        _queue.clear();
        _mutex.unlock();
    }

    void push(const T& t)
    {
        _mutex.lock();
        _queue.enqueue(t);
        _mutex.unlock();
    }

    T pull()
    {
        _mutex.lock();
        T i = _queue.dequeue();
        _mutex.unlock();
        return i;
    }

private:

    QQueue<T> _queue;
    QMutex _mutex;
    int _max;
};
