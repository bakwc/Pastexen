#include "psaver.h"
#include <QTime>
#include <QFile>
#include "psetting.h"

pSaver* pSaver::pThis = 0;

pSaver::pSaver(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
    this->start(QThread::LowPriority);

    pThis = this;
}

void pSaver::save(const QByteArray &data, const QString& type)
{
    int i = 10;
    QFile file;
    QString filename;
    QString typeFolder(pSetting::types()[type]);

    do {
        filename = randName(pSetting::fileNameLenght()) + '.' + type;
        const QString path = typeFolder + filename;
        file.setFileName(path);

//        qDebug() << path;

    } while(file.exists() && --i);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot create file:" << file.fileName();
    }

    file.write(data);
}


QString pSaver::randName(int count)
{
    QString str;

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    qsrand(QTime().msecsTo(QTime::currentTime()));
    for (int i = 0; i < count; ++i) {
        str += alphanum[qrand() % (sizeof(alphanum) - 1)];
    }

    return str;
}
