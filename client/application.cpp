#include <QIcon>
#include <QKeySequence>
#include <QFile>
#include <QTemporaryFile>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDebug>
#include <QClipboard>
#include <QMessageBox>
#include <QBuffer>
#include "application.h"
#include "imageselectwidget.h"
#include "ui_config.h"
#include "defines.h"
#include "languageselectdialog.h"

Application::Application(int argc, char *argv[]):
    QxtApplication(argc, argv), _configWidget(0), _trayIcon(0), _trayIconMenu(0/*new QMenu*/),
    _shortcutScreenFull(0/*new QxtGlobalShortcut*/), _shortcutScreenPart(0/*new QxtGlobalShortcut*/),
    _shortcutTextShare(0/*new QxtGlobalShortcut*/), _network(0), _settings(0)
{
//    _settings = new QSettings(SETTINGS_FILE, QSettings::IniFormat, this);
//    _shortcutScreenFull = new QxtGlobalShortcut;
//    _shortcutScreenPart = new QxtGlobalShortcut;
//    _shortcutTextShare  = new QxtGlobalShortcut;
}

Application::~Application()
{
    _trayIcon->hide();
    _settings->sync();
    delete _shortcutScreenPart;
    delete _shortcutScreenFull;
    delete _shortcutTextShare;
    delete _trayIconMenu;
}

void Application::pxAppInit()
{
    _settings = new QSettings(SETTINGS_FILE, QSettings::IniFormat, this);

    _configWidget = new ConfigWidget(_settings);
    QObject::connect(_configWidget, SIGNAL(settingsChanged()), this, SLOT(setupHotkeys()));

    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    QString codeHotkey = _settings->value("general/texthotkey", DEFAULT_HOTKEY_CODE).toString();

    _configWidget->init(fullHotkey, partHotkey, codeHotkey);

    _shortcutScreenFull = new QxtGlobalShortcut;
    QObject::connect(_shortcutScreenFull, SIGNAL(activated()), SLOT(processScreenshotFull()));
//    if (!_shortcutScreenFull->setShortcut(QKeySequence(fullHotkey)))
//        qDebug() << "Error activating hotkey:" << fullHotkey;          // Shortcut for full screen

    _shortcutScreenPart = new QxtGlobalShortcut;
    QObject::connect(_shortcutScreenPart, SIGNAL(activated()), SLOT(processScreenshotPart()));
//    if (!_shortcutScreenPart->setShortcut(QKeySequence(partHotkey)))
//        qDebug() << "Error activating hotkey:" << partHotkey;        // Shortcut for part of the screen

    _shortcutTextShare = new QxtGlobalShortcut;
    QObject::connect(_shortcutTextShare, SIGNAL(activated()), SLOT(processCodeShare()));
//    if (!_shortcutTextShare->setShortcut(QKeySequence(codeHotkey)))
//        qDebug() << "Error activating hotkey:" << codeHotkey;          // Shortcut for text share


    _trayIconMenu = new QMenu;
    _trayIconMenu->addAction(tr("About"), this, SLOT(aboutDialog()));
    _trayIconMenu->addAction("", this, SLOT(processCodeShare()));
    _trayIconMenu->addAction("", this, SLOT(processScreenshotFull()));
    _trayIconMenu->addAction("", this, SLOT(processScreenshotPart()));
    _trayIconMenu->addAction(tr("Configure"), _configWidget, SLOT(show()));
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(tr("Exit"), this, SLOT(quit()));                   // Tray menu

    setupHotkeys();

//    QIcon icon(":/icons/icon.png");
    _trayIcon = new QSystemTrayIcon(QIcon(":/icons/icon.png"), this);
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    _trayIcon->setContextMenu(_trayIconMenu);                                   // Tray icon

    _network = new Network(_settings, this);
    connect(_network, SIGNAL(linkReceived(QString)), SLOT(linkAvaliable(QString)));     // Network

    this->setQuitOnLastWindowClosed(false);

    qDebug() << Q_FUNC_INFO;
    _trayIcon->show();
    QFile file(SETTINGS_FILE);

    if (!file.exists())
        _configWidget->show();
}

void Application::processScreenshot(bool isFullScreen)
{
    qDebug() << Q_FUNC_INFO << 1;

    QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    if (!isFullScreen) {
        qDebug() << Q_FUNC_INFO << 2;

        ImageSelectWidget imageSelectDialog(&pixmap);
        qDebug() << Q_FUNC_INFO << 3;

        imageSelectDialog.setWindowState(Qt::WindowFullScreen);
        qDebug() << Q_FUNC_INFO << 4;

        if (!imageSelectDialog.exec())
            return;
    }

    auto imagetype = _settings->value("general/imagetype", DEFAULT_IMAGE_TYPE).toString();

    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QFile::WriteOnly);
    pixmap.save(&buffer, imagetype.toAscii().constData());
    buffer.close();
    _network->upload(imageBytes, imagetype);
}

void Application::processCodeShare()
{
    qDebug() << "Sharing code!";

    bool showsourcedialog = _settings->value("general/showsourcedialog", DEFAULT_SHOW_SOURCES_CONF_DIALOG).toBool();
    if (showsourcedialog) {
        LanguageSelectDialog dialog(_settings);
        dialog.exec();
    }

    auto sourcestype = _settings->value("general/sourcetype", DEFAULT_SOURCES_TYPE).toString();
    auto text = QApplication::clipboard()->text();
    _network->upload(text.toUtf8(),sourcestype);
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
    qDebug() << Q_FUNC_INFO;
    QApplication::clipboard()->setText(link);
    _trayIcon->showMessage(tr("Done!"), tr("File shared!"), QSystemTrayIcon::Information, 6500);
}

void Application::aboutDialog()
{
    QMessageBox::information(NULL, APP_NAME,
                             tr("Utility for easy screenshots and code sharing"));
}

void Application::setupHotkeys()
{
    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    QString codeHotkey = _settings->value("general/texthotkey", DEFAULT_HOTKEY_CODE).toString();

    if (!_shortcutScreenFull->setShortcut(QKeySequence(fullHotkey)))
        qDebug() << "Error activating hotkey:" << fullHotkey;          // Shortcut for full screen

    if (!_shortcutScreenPart->setShortcut(QKeySequence(partHotkey)))
        qDebug() << "Error activating hotkey:" << partHotkey;        // Shortcut for part of the screen

    if (!_shortcutTextShare->setShortcut(QKeySequence(codeHotkey)))
        qDebug() << "Error activating hotkey:" << codeHotkey;          // Shortcut for text share

//    _trayIconMenu->actionAt(QPoint(0, 1))->setText(tr("Text share (%1)").arg(codeHotkey));
//    _trayIconMenu->addAction(tr("Full s-shot (%1)").arg(fullHotkey)
//    _trayIconMenu->addAction(tr("Half s-shot (%1)").arg(partHotkey)

    QList<QAction*> actsList = _trayIconMenu->actions();
    actsList[1]->setText(tr("Text share (%1)").arg(codeHotkey));
    actsList[2]->setText(tr("Full s-shot (%1)").arg(fullHotkey));
    actsList[3]->setText(tr("Half s-shot (%1)").arg(partHotkey));
}
