#include "psaver.h"
#include <QTime>
#include <QFile>
#include "psetting.h"
#include "psocket.h"

pSaver* pSaver::pThis = 0;

pSaver::pSaver() :
    QThread(0)
{
    moveToThread(this);
    this->start(QThread::LowPriority);

    pThis = this;
}

void pSaver::save(const QByteArray &data, const QString& type, const QString& filename)
{
    int i = 10;
    QFile file;
//    QString filename;
    QString typeFolder(Settings::types()[type]);

//        filename = randName(pSetting::fileNameLenght()) + '.' + type;
    const QString path = typeFolder + filename;
    file.setFileName(path);

//        qDebug() << path;

//    } while(file.exists() && --i);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot create file:" << file.fileName();
        return;
    }

    file.write(data);
}
