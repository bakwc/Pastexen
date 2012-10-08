#pragma once

#include <QSettings>
#include <QHostAddress>
#include <QStringList>
#include <QMap>

class Settings
{
public:
    Settings(const QString &filename);

    static const QHostAddress& host() { return inst()->_host; }
    static int                 port() { return inst()->_port; }
    static int                 threads() { return inst()->_threads; }
    static int                 fileNameLenght() { return inst()->_fileNameLength; }
    static const QMap<QString, QString>& types() { return inst()->_types; }
    static const QMap<QString, QString>& prefixes() { return inst()->_prefixes; }
    static const QString&      imageLinkPrefix() { return inst()->_imageLinkPrefix; }
    static const QString&      sourceLinkPrefix() { return inst()->_sourceLinkPrefix; }

private:
    QSettings   _set;
    static Settings    *pThis;

    QHostAddress    _host;
    int             _port;
    int             _threads;
    int             _fileNameLength;
    QMap<QString, QString>  _types;
    QMap<QString, QString>  _prefixes;
    QString         _imageLinkPrefix;
    QString         _sourceLinkPrefix;

    static const Settings* inst() { return pThis; }
};
