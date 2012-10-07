#include "psetting.h"

Settings* Settings::pThis = 0;

Settings::Settings(const QString &filename) :
    _set(filename, QSettings::IniFormat)
{
    if (_set.status() != QSettings::NoError) {
        qDebug() << "Setting file is not exist. Server run with default parameters";
    }

    _host       = _set.value("host", "0.0.0.0").toString();
    _port       = _set.value("port", 9876).toInt();
    _threads    = _set.value("threads", 8).toInt();
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

    //for(const QString& key : _imageTypes) {
    //    _types.insert(key, _imageDir);
    //}

    //for(const QString& key : _sourceTypes) {
    //    _types.insert(key, _sourceDir);
    //}

    pThis = this;

    qDebug() << "Settings:";
    qDebug() << " Host:" << host().toString();
    qDebug() << " Port:" << port();
    qDebug() << " Threads:" << threads();
    qDebug() << " File name lenght:" << fileNameLenght();
    qDebug() << " Types:" << types();
    qDebug() << " Prefixes:" << prefixes();
}
