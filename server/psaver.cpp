#include "psaver.h"
#include <QCoreApplication>
#include <QTime>
#include <QFile>
#include "psetting.h"
#include "psocket.h"
#include <QEvent>
#include <QtAlgorithms>
#include <QDir>
#include <ctime>

pSaver* pSaver::pThis = 0;

pSaver::pSaver()
    : QThread(0)
    , _redis(0)
{
    moveToThread(this);

    this->start(QThread::LowPriority);

    findFiles();

    pThis = this;
}

void pSaver::save(const QByteArray &data, const QString& type, const QString& uuid)
{
#ifdef FUNC_DEBUG
    qDebug() << "\n    " << Q_FUNC_INFO;
#endif

    if (_redis == 0) {
        _redis = new TRedisClient(this);
        _redis->SetTimeout(1200);
    }

    QFile file;
    QString filename, path;
    if (Settings::types().find(type) == Settings::types().end()) {
        return;
    }
    QString typeFolder(Settings::types()[type]);

    do {
        filename = randName(Settings::fileNameLenght()) + '.' + type;
        path = typeFolder + filename;
        file.setFileName(path);
    } while(_files.contains(path));

    _files.insert(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "\nCannot create file:" << file.fileName();
        return;
    }

    if (uuid.length() != 0) {
        if (!_redis->Connect("127.0.0.1", 6379)) {
            qDebug() << "redis server unavailable";
        } else {

            QStringList record = _redis->Query("INCR records_count");
            if (record.size() == 2) {
                QString recordNumber = record[1];
                QString fileType = "text";
                if (type == "jpg" || type == "png") {
                    fileType = "image";
                }

                int timestamp = std::time(0);

                size_t n = filename.lastIndexOf(".");
                QString clearName = filename.left(n);

                _redis->Query("HSET file_" + recordNumber + " type " + fileType);
                _redis->Query("HSET file_" + recordNumber + " name " + clearName);
                _redis->Query("HSET file_" + recordNumber + " extension " + type);
                _redis->Query("HSET file_" + recordNumber + " path " + path);
                _redis->Query("HSET file_" + recordNumber + " uuid " + uuid);
                _redis->Query("HSET fileid " + filename + " " + recordNumber);
                _redis->Query(QString("HSET file_path %1 %2")
                         .arg(path)
                         .arg(recordNumber));

                _redis->Query(QString("HSET file_%1 timestamp %2")
                         .arg(recordNumber)
                         .arg(timestamp));

                _redis->Query(QString("ZADD uuid_%1 %2 file_%3")
                         .arg(uuid)
                         .arg(timestamp)
                         .arg(recordNumber));
                _redis->Query("SADD thmb_tasks " + recordNumber);
                _redis->Disconnect();
            } else {
                qDebug() << "Wrong redis response";
            }
        }
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
