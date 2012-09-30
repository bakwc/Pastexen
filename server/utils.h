#pragma once

#include <QFile>
#include <QDateTime>
#include <QMap>

#include <stdexcept>

QString randName(int count) {
    QString str;

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < count; ++i) {
        str += alphanum[qrand() % (sizeof(alphanum) - 1)];
    }

    return str;
}


QString saveToFile(const QByteArray& data, const QString& type, const QMap<QString, QString>& types, int fnL) {
    QString path(types[type]);
    const QString filename = randName(fnL) + '.' + type;
    path += '/' + filename;

    qDebug() << path;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Cannot create file");
    }

    file.write(data);
    return filename;
}

QString getValue(const QString& data, const QString& key)
{
    int start = data.indexOf(key);

    QString res = data.mid(start + key.length() + 1);
    res = res.left(res.indexOf("\n"));

    return res;
}

