#pragma once

#include <QString>
#include <QTime>

inline QString getValue(const QString& data, const QString& key)
{
    int start = data.indexOf(key);

    QString res = data.mid(start + key.length() + 1);
    res = res.left(res.indexOf("\n"));

    return res;
}


inline QString GenerateUUID()
{
    QTime now = QTime::currentTime();
    qsrand(now.msec());

    QString uuid(24 * 2);
    for (int i = 0; i < 24 * 2; i++) {
        int c = qrand() % 16;
        Q_ASSERT(c >= 0);
        uuid[i] = c < 10 ? '0' + c : 'A' + (c - 10);
    }
    return uuid;
}
