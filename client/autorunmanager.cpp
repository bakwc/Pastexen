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

#include "defines.h"
#include <QFileInfo>
#include <QDir>

static const QString MAC_STARTUP_TEMPLATE =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
        "<plist version=\"1.0\">\n"
        "<dict>\n"
        "    <key>Label</key>\n"
        "    <string>%1</string>\n"
        "    <key>ProgramArguments</key>\n"
        "    <array>\n"
        "        <string>%2</string>\n"
        "    </array>\n"
        "    <key>ProcessType</key>\n"
        "    <string>Interactive</string>\n"
        "    <key>RunAtLoad</key>\n"
        "    <true/>\n"
        "    <key>KeepAlive</key>\n"
        "    <false/>\n"
        "</dict>\n"
        "</plist>\n";

AutorunManager::AutorunManager(QObject *parent):
    QObject(parent)
{
    _currentStatus = checkInstallation();
}

void AutorunManager::install()
{
    QString pathToExecutable = QCoreApplication::applicationFilePath();
    QString pathToStartupScript = QDir::homePath() + "/Library/LaunchAgents/" + MAC_APP_ID + ".plist";
    QString data = MAC_STARTUP_TEMPLATE.arg(MAC_APP_ID).arg(pathToExecutable);

    QFile file(pathToStartupScript);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    file.write(data.toLatin1());
    file.close();
    _currentStatus = checkInstallation();
}

AutorunStatus AutorunManager::checkInstallation()
{
    QString pathToExecutable = QCoreApplication::applicationFilePath();
    QString pathToStartupScript = QDir::homePath() + "/Library/LaunchAgents/" + MAC_APP_ID + ".plist";
    QString data = MAC_STARTUP_TEMPLATE.arg(MAC_APP_ID).arg(pathToExecutable);
    QFile file(pathToStartupScript);
    if (!file.open(QIODevice::ReadOnly))
        return NotInstalled;
    QString content = file.readAll();
    if (content == data)
        return ForCurrentUser;
    return NotInstalled;
}

void AutorunManager::uninstall()
{
    QString pathToStartupScript = QDir::homePath() + "/Library/LaunchAgents/" + MAC_APP_ID + ".plist";
    QFile::remove(pathToStartupScript);
    _currentStatus = checkInstallation();
}

#endif

AutorunStatus AutorunManager::isInstalled() const
{
    return _currentStatus;
}
