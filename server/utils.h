#pragma once

#include <QFile>
#include <QDateTime>
#include <QMap>

#include <stdexcept>

QString randName(int count)
{
    QString str;

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    qsrand(time(NULL));
    for (int i = 0; i < count; ++i) {
        str += alphanum[qrand() % (sizeof(alphanum) - 1)];
    }

    return str;
}


QString saveToFile(const QByteArray& data, const QString& type, const QMap<QString, QString>& types, int fnL)
{
    int i = 10;
    QFile file;
    QString filename;
    QString path(types[type]);

    do {
        filename = randName(fnL) + '.' + type;
        path += '/' + filename;
        file.setFileName(path);
    } while(file.exists() && --i);

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

