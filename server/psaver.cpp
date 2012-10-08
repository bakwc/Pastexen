#include "psaver.h"
#include <QCoreApplication>
#include <QTime>
#include <QFile>
#include "psetting.h"
#include "psocket.h"
#include <QEvent>

pSaver* pSaver::pThis = 0;

pSaver::pSaver() :
    QThread(0)
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
    QString typeFolder(Settings::types()[type]);

    filename = randName(Settings::fileNameLenght()) + '.' + type;
    const QString path = typeFolder + filename;
    file.setFileName(path);

//        qDebug() << path;

//    } while(file.exists() && --i);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot create file:" << file.fileName();
        return;
    }

    file.write(data);

    qApp->postEvent(sender(), new SendLinkEvent( Settings::prefixes()[type] + filename ));
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


//void pSaver::customEvent(QEvent *ev)
//{
//    if (ev->type() == SaveEvent::TYPE) {
////        qDebug() << ((SaveEvent*)ev)->fileType();
//        SaveEvent *event = (SaveEvent*)ev;

//        save(event->data(), event->fileType(), event->sender());
//    }
//}
