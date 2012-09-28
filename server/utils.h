#ifndef UTILS_H
#define UTILS_H

#include <QFile>
#include <QDateTime>
#include <QtAlgorithms>

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


void saveToFile(const QByteArray& data, const QString& type) {
    qDebug() << "File type:" << type << "Data:" << data;

    QString filename;
    filename += randName(6) + type;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    file.write(data);
}

QString getValue(const QString& data, const QString& key)
{
    int start = data.indexOf(key);

    QString res = data.mid(start + key.length() + 1);
    res = res.left(res.indexOf("\n"));

    return res;
}

#endif // UTILS_H
