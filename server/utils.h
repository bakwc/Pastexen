#pragma once

#include <QFile>
#include <QDateTime>
#include <QMap>
#include <QMutex>

QString getValue(const QString& data, const QString& key)
{
    int start = data.indexOf(key);

    if (start == -1) {
        return "";
    }

    QString res = data.mid(start + key.length() + 1);
    res = res.left(res.indexOf("\n"));

    return res;
}
