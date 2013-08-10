#include "psetting.h"
#include <QThread>

Settings* Settings::pThis = 0;

Settings::Settings(const QString &filename) :
    _set(filename, QSettings::IniFormat)
{
    if (_set.status() != QSettings::NoError) {
        qDebug() << "Setting file is not exist. Server run with default parameters\n";
    }

    _host       = _set.value("host", "0.0.0.0").toString();
    _port       = _set.value("port", 9876).toInt();
    _redis_port = _set.value("redis_port", 6379).toInt();
    _threads    = _set.value("threads", QThread::idealThreadCount() - 2).toInt();
    _fileNameLength = _set.value("file_name_length", 5).toInt();
    QStringList _imageTypes = _set.value("image_types", QStringList() << "jpg" << "png").toStringList();
    QStringList _sourceTypes= _set.value("source_types", QStringList() << "c" << "cpp" << "txt" << "py").toStringList();
    QString     _imageDir   = _set.value("image_dir", "imageDir/").toString();
    QString     _sourceDir  = _set.value("source_dir", "sourceDir/").toString();
    _imageLinkPrefix = _set.value("image_link_prefix", "http://prefix.com/image/").toString();
    _sourceLinkPrefix = _set.value("source_link_prefix", "http://prefix.com/sorces/").toString();

    for (auto i = _imageTypes.begin(); i != _imageTypes.end(); ++i) {
       _types.insert(*i, _imageDir);
       _prefixes.insert(*i, _imageLinkPrefix);
    }

    for (auto i = _sourceTypes.begin(); i != _sourceTypes.end(); ++i) {
       _types.insert(*i, _sourceDir);
       _prefixes.insert(*i, _sourceLinkPrefix);
    }

    pThis = this;

    qDebug() << "\nSettings:"
             << "\n Host:" << host().toString()
             << "\n Port:" << port()
             << "\n Redis port: " << redis_port()
             << "\n Threads:" << threads()
             << "\n File name lenght:" << fileNameLenght()
             << "\n Types:" << types()
             << "\n Prefixes:" << prefixes();
}
