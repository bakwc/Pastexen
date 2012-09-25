#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QString>
#include <QHttp>
#include <QSslError>
#include <QSettings>
#include "defines.h"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QSettings *settings, QObject *parent = 0);
    void upload(const QString &fileName);
signals:
    void linkReceived(const QString &link);
private slots:
    void httpDone(bool isError);
    void onSslErrors(QList<QSslError>);
private:
    QString getAuth();
    QByteArray readFile(const QString& fileName);
    inline QString getDir() { return _settings->value("general/directory", DEFAULT_DIR).toString(); }
    void createDir(const QString& dir);
    void shareFile(const QString& file);
private:
    QHttp *_httpConnection;
    QSettings *_settings;
    qint32 _stage;
    bool _ready;
};

#endif // NETWORK_H
