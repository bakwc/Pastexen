#ifndef PSETTING_H
#define PSETTING_H

#include <QSettings>
#include <QHostAddress>
#include <QStringList>
#include <QMap>

class pSetting
{
public:
    pSetting(const QString &filename);

    static const QHostAddress& host() { return inst()->_host; }
    static int                 port() { return inst()->_port; }
    static int                 threads() { return inst()->_threads; }
    static int                 fileNameLenght() { return inst()->_fileNameLength; }
    static const QMap<QString, QString>& types() { return inst()->_types; }
    static const QString&      imageLinkPrefix() { return inst()->_imageLinkPrefix; }
    static const QString&      sourceLinkPrefix() { return inst()->_sourceLinkPrefix; }

private:
    QSettings   _set;
    static pSetting    *pThis;

    QHostAddress    _host;
    int             _port;
    int             _threads;
    int             _fileNameLength;
    QMap<QString, QString>  _types;
    QString         _imageLinkPrefix;
    QString         _sourceLinkPrefix;

    static pSetting* inst() { return pThis; }
};

#endif // PSETTING_H
