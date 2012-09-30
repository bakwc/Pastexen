#include "psetting.h"

pSetting* pSetting::pThis = 0;

pSetting::pSetting(const QString &filename) :
    _set(filename, QSettings::IniFormat)
{
    if (_set.status() != QSettings::NoError) {
        qDebug() << "Setting file is not exist";
    }

    _host       = _set.value("host", "0.0.0.0").toString();
    _port       = _set.value("port", 9876).toInt();
    _threads    = _set.value("threads", 1).toInt();
    _fileNameLength = _set.value("file_name_length", 5).toInt();
    QStringList _imageTypes = _set.value("image_types", QStringList() << "jpg" << "png").toStringList();
    QStringList _sourceTypes= _set.value("source_types", QStringList() << "c" << "cpp" << "txt" << "py").toStringList();
    QString     _imageDir   = _set.value("image_dir", "imageDir/").toString();
    QString     _sourceDir  = _set.value("source_dir", "sourceDir/").toString();

    for(const QString& key : _imageTypes) {
        _types.insert(key, _imageDir);
    }

    for(const QString& key : _sourceTypes) {
        _types.insert(key, _sourceDir);
    }

    pThis = this;

    qDebug() << "Settings:";
    qDebug() << " Host:" << host().toString();
    qDebug() << " Port:" << port();
    qDebug() << " Threads:" << threads();
    qDebug() << " File name lenght:" << fileNameLenght();
    qDebug() << " Types:" << types();
}
