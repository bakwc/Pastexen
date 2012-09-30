#include <QIcon>
#include <QKeySequence>
#include <QFile>
#include <QTemporaryFile>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDebug>
#include <QClipboard>
#include <QMessageBox>
#include "application.h"
#include "imageselectwidget.h"
#include "ui_config.h"
#include "defines.h"

/*
#define _WIN32_WINNT 0x0500
#include <windows.h>
HWND WINAPI GetConsoleWindow(void);
*/

Application::Application(int argc, char *argv[]):
    QxtApplication(argc, argv)
{
    /*
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, 0);
    */

    _settings = new QSettings(SETTINGS_FILE, QSettings::IniFormat);
    _settings->setParent(this);                                                         // Settings

    _configUi = new Ui::Form;
    _configWidget = new ConfigWidget(_configUi, _settings);
    _configUi->setupUi(_configWidget);
    _configWidget->init();
    _configWidget->setFixedSize(_configWidget->size());
    connect(_configUi->cancelButton, SIGNAL(clicked()), _configWidget, SLOT(hide()));
    connect(_configUi->applyButton, SIGNAL(clicked()), _configWidget, SLOT(applyChanges()));    // Config window

    _trayIconMenu = new QMenu;
    _trayIconMenu->addAction(tr("About"), this, SLOT(aboutDialog()));
    _trayIconMenu->addAction(tr("Full s-shot (%2)").arg(DEFAULT_HOTKEY_FULL), this, SLOT(processScreenshotFull()));
    _trayIconMenu->addAction(tr("Half s-shot (%1)").arg(DEFAULT_HOTKEY_PART), this, SLOT(processScreenshotPart()));
    _trayIconMenu->addAction(tr("Configure"), _configWidget, SLOT(show()));
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(tr("Exit"), this, SLOT(quit()));                   // Tray menu

    QIcon icon(":/icons/yascreens.png");
    _trayIcon = new QSystemTrayIcon(icon, this);
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    _trayIcon->setContextMenu(_trayIconMenu);                                   // Tray icon

    _shortcutScreenFull = new QxtGlobalShortcut;
    QObject::connect(_shortcutScreenFull, SIGNAL(activated()), SLOT(processScreenshotFull()));
    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    if (!_shortcutScreenFull->setShortcut(QKeySequence(fullHotkey)))
        qDebug("Error activating hotkey..");                                        // Shortcut for full screen

    _shortcutScreenPart = new QxtGlobalShortcut;
    QObject::connect(_shortcutScreenPart, SIGNAL(activated()), SLOT(processScreenshotPart()));
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    if (!_shortcutScreenPart->setShortcut(QKeySequence(partHotkey)))
        qDebug("Error activating hotkey..");                                        // Shortcut for part of the screen

    _network = new Network(_settings, this);
    connect(_network, SIGNAL(linkReceived(QString)), SLOT(linkAvaliable(QString)));     // Network

    this->setQuitOnLastWindowClosed(false);
}

Application::~Application()
{
    _trayIcon->hide();
    _settings->sync();
    delete _shortcutScreenPart;
    delete _shortcutScreenFull;
    delete _trayIconMenu;
    delete _configUi;
}

void Application::init()
{
    qDebug() << Q_FUNC_INFO;
    _trayIcon->show();
    QFile file(SETTINGS_FILE);
    if (!file.exists())
        _configWidget->show();
}

void Application::processScreenshot(bool isFullScreen)
{
    QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

    if (!isFullScreen)
    {
        ImageSelectWidget imageSelectDialog(&pixmap);
        if (!imageSelectDialog.exec())
            return;
    }

    QTemporaryFile tempFile;
    if (!tempFile.open())
    {
        qDebug() << "Error opening temporary file!";
        return;
    }
    tempFile.close();

    pixmap.save(tempFile.fileName(),"png");

    _network->upload(tempFile.fileName());
}

void Application::trayIconClicked(const QSystemTrayIcon::ActivationReason &button)
{
    switch (button)
    {
    case QSystemTrayIcon::Trigger:
        _configWidget->show();
        break;
    case QSystemTrayIcon::MiddleClick:
        this->quit();
        break;
    default:
        break;
    }
}

void Application::linkAvaliable(const QString &link)
{
    QApplication::clipboard()->setText(link);
    _trayIcon->showMessage(tr("Done!"), tr("Screenshot uploaded!"), QSystemTrayIcon::Information, 6500);
}

void Application::aboutDialog()
{
    QMessageBox::information(NULL, APP_NAME,
       tr("Utility for easy screenshot making and uploading to yandex.disc"));
}
