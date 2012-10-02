#ifndef SAVER_H
#define SAVER_H

#include <QThread>
#include <QString>
#include <QByteArray>

class pSaver : public QThread
{
    Q_OBJECT
public:
    explicit pSaver(QObject *parent = 0);
    static pSaver* inst() { return pThis; }

signals:
    
public slots:
    void save(const QByteArray& data, const QString &type);

private:
    static pSaver  *pThis;
    static QString randName(int count);
};

#endif // SAVER_H
