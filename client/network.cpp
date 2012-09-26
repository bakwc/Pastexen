#include <QHttpRequestHeader>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QtTest/QTest>
#include "network.h"

Network::Network(QSettings *settings, QObject *parent) :
    QObject(parent),
    _settings(settings)
{
    _httpConnection = new QHttp(this);
    connect(_httpConnection, SIGNAL(done(bool)), SLOT(httpDone(bool)));

    #ifndef QT_NO_OPENSSL
    QObject::connect(_httpConnection, SIGNAL(sslErrors(QList<QSslError>)),
                     SLOT(onSslErrors(QList<QSslError>)));
    #else
    qDebug() << "SSL not supported!";
    exit(1);
    #endif
}

void Network::upload(const QString &filename)
{
    auto auth = getAuth();
    auto data = readFile(filename);
    auto dir = getDir();

    createDir(dir);

    QString imgName=QString("%1_%2.png")
            .arg(QDate::currentDate().toString("yyyy-MM-dd"))
            .arg(QTime::currentTime().toString("HH:mm:ss"));

    QHttpRequestHeader header(QString("PUT %1%2 HTTP/1.1").arg(dir).arg(imgName));
    header.setValue("Host", "webdav.yandex.ru");
    header.addValue("Accept", "*/*");
    header.addValue("Authorization", QString("Basic %1").arg(auth));
    header.addValue("Expect", "100-continue");
    header.setContentType("application/binary");
    header.setContentLength(data.length());

    _ready=false;
    _httpConnection->setHost(QString("webdav.yandex.ru"), QHttp::ConnectionModeHttps, 443);
    _httpConnection->request(header, data);
    while (!_ready) QTest::qWait(200);

    shareFile(QString("%1%2").arg(dir).arg(imgName));
}

QString Network::getAuth()
{
    auto login = _settings->value("general/login", DEFAULT_LOGIN).toString();
    auto pass = _settings->value("general/password", DEFAULT_PASS).toString();
    auto auth = QString("%1:%2").arg(login).arg(pass);
    return auth.toLocal8Bit().toBase64();
}

QByteArray Network::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening temporary file!";
        exit(1);
    }
    return file.readAll();
}

void Network::httpDone(bool isError)
{
    if (isError)
        qDebug() << "Error: " << _httpConnection->errorString();
    else
    {
        QHttpResponseHeader resp=_httpConnection->lastResponse();
        QString location = resp.value("location");
        if (location.size()>0)
            emit linkReceived(location);
    }
    _ready=true;
}

void Network::onSslErrors(QList<QSslError>)
{
    qDebug() << "SSL Errors!";
}

void Network::createDir(const QString& dir)
{
    if (dir=="/") return;
    auto auth = getAuth();
    QHttpRequestHeader header(QString("MKCOL %1 HTTP/1.1").arg(dir));
    header.setValue("Host", "webdav.yandex.ru");
    header.addValue("Accept", "*/*");
    header.addValue("Authorization", QString("Basic %1").arg(auth));

    _ready=false;
    _httpConnection->setHost(QString("webdav.yandex.ru"), QHttp::ConnectionModeHttps, 443);
    _httpConnection->request(header);
    while (!_ready) QTest::qWait(200);
}

void Network::shareFile(const QString& file)
{
    auto auth = getAuth();

    QHttpRequestHeader header(QString("POST %1?publish HTTP/1.1").arg(file));
    header.setValue("Host", "webdav.yandex.ru");
    header.addValue("Accept", "*/*");
    header.addValue("Authorization", QString("Basic %1").arg(auth));

    _ready=false;
    _httpConnection->setHost(QString("webdav.yandex.ru"), QHttp::ConnectionModeHttps, 443);
    _httpConnection->request(header);
    while (!_ready) QTest::qWait(200);
}
