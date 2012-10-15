#include "psaver.h"
#include <QCoreApplication>
#include <QTime>
#include <QFile>
#include "psetting.h"
#include "psocket.h"
#include <QEvent>
#include <QtAlgorithms>
#include <QDir>

pSaver* pSaver::pThis = 0;

pSaver::pSaver() :
    QThread(0)
{
    moveToThread(this);
    this->start(QThread::LowPriority);

    findFiles();

    pThis = this;
}

void pSaver::save(const QByteArray &data, const QString& type)
{
#ifdef FUNC_DEBUG
    qDebug() << "\n    " << Q_FUNC_INFO;
#endif

    int i = 10;
    QFile file;
    QString filename, path;
    QString typeFolder(Settings::types()[type]);

    do {
        filename = randName(Settings::fileNameLenght()) + '.' + type;
        path = typeFolder + filename;
        file.setFileName(path);
    } while(_files.contains(path) && --i);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "\nCannot create file:" << file.fileName();
        return;
    }

#ifdef FUNC_DEBUG
    qDebug() << "\n    "  << Q_FUNC_INFO << "Write: path:" << path << ", size:" << data.size();
#endif

    file.write(data);

#ifdef FUNC_DEBUG
    qDebug() << "\n    "  << Q_FUNC_INFO << "postEvent()";
#endif

    qApp->postEvent(sender(), new SendLinkEvent( Settings::prefixes()[type] + filename ));

#ifdef FUNC_DEBUG
    qDebug() << "\n    "  << Q_FUNC_INFO << "end";
#endif
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

void pSaver::findFiles()
{
    QDir dir;
    QStringList list = unique(Settings::types().values());

    for (auto it = list.begin(); it != list.end(); ++it) {
        dir.setPath(*it);
        QStringList entry = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (auto eIt = entry.begin(); eIt != entry.end(); ++eIt) {
            const QString str = *it + *eIt;
            _files.insert(str);
        }
    }
}

QStringList pSaver::unique(const QStringList &list)
{
    QStringList l;

    for (auto it = list.begin(); it != list.end(); ++it) {
        if (!l.count(*it))
            l.push_back(*it);
    }

    return l;
}
