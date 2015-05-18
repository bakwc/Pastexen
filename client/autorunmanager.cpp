#include "autorunmanager.h"
#include <QDebug>

#ifdef Q_OS_WIN

AutorunManager::AutorunManager(QObject *parent) :
    QObject(parent)
{
    _currentUser = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat, this);
    _localMachine = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat, this);
    _currentStatus = checkInstallation();
}

void AutorunManager::install(bool forAllUsers)
{
    (forAllUsers ? _localMachine : _currentUser)->setValue("Pastexen", QCoreApplication::applicationFilePath().replace('/','\\'));
    _currentStatus = checkInstallation();
}

void AutorunManager::uninstall()
{
    _currentUser->remove("Pastexen");
    _localMachine->remove("Pastexen");
    _currentStatus = NotInstalled;
}

AutorunStatus AutorunManager::checkInstallation()
{
    if (_localMachine->allKeys().contains("Pastexen"))
        return ForAllUsers;

    else if (_currentUser->allKeys().contains("Pastexen"))
        return ForCurrentUser;

    else
        return NotInstalled;
}

#elif defined(Q_OS_LINUX)

AutorunManager::AutorunManager(QObject *parent) :
    QObject(parent)
{
    _path = QDir::homePath()+"/.config/autostart/Pastexen.desktop";
    _currentStatus = checkInstallation();
}

void AutorunManager::install()
{
    QFile autorunEntry(_path);
    autorunEntry.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&autorunEntry);
    stream << "[Desktop Entry]\n";
    stream << "Type=Application\n";
    stream << QString("Exec=%1\n").arg(QCoreApplication::applicationFilePath());
    stream << "Name=Pastexen\n";
    stream << "GenericName=Utility for quick screenshot and source code publishing\n";
    stream << "Terminal=false\n";
    stream << "Categories=Internet\n";
    
    autorunEntry.close();
    _currentStatus = ForCurrentUser;
}

void AutorunManager::uninstall()
{
    QFile::remove(_path);
    _currentStatus = NotInstalled;
}

AutorunStatus AutorunManager::checkInstallation()
{
    if (QFile::exists(_path))
        return ForCurrentUser;
    else
        return NotInstalled;
}

#elif defined(Q_OS_MAC)

AutorunManager::AutorunManager(QObject *parent):
    QObject(parent)
{
}

void AutorunManager::install()
{
}

void AutorunManager::uninstall()
{
}

#endif

AutorunStatus AutorunManager::isInstalled() const
{
    return _currentStatus;
}
