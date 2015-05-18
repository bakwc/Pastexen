#ifndef AUTORUNMANAGER_H
#define AUTORUNMANAGER_H

#include <QObject>
#include <QString>
#include <QCoreApplication>
#include <QStringList>

#ifdef Q_OS_WIN
    #include <QSettings>
#elif defined(Q_OS_LINUX)
    #include <QDir>
    #include <QFile>
    #include <QTextStream>
#endif

enum AutorunStatus {
    NotInstalled = 0,
    ForAllUsers,
    ForCurrentUser
};

class AutorunManager : public QObject
{
    Q_OBJECT
public:
    explicit AutorunManager(QObject *parent = 0);
#ifdef Q_OS_WIN
    void install(bool forAllUsers=false);
#elif defined(Q_OS_LINUX)
    void install();
#elif defined(Q_OS_MAC)
    void install();
#endif
    void uninstall();
    AutorunStatus isInstalled() const;
    AutorunStatus checkInstallation();

private:
    AutorunStatus _currentStatus;
#ifdef Q_OS_WIN
    QSettings *_localMachine;
    QSettings *_currentUser;
#elif defined(Q_OS_LINUX)
    QString _path;
#endif

signals:

public slots:

};

#endif // AUTORUNMANAGER_H
