#pragma once

#include <QThread>
#include <QString>

class Thread : public QThread
{
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};

QString getValue(const QString& data, const QString& key);
