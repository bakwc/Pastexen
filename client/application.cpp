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
#include <QLocalSocket>
#include <QTimer>
#include <QDir>
#include <QScreen>
#include "application.h"
#include "imageselectwidget.h"
#include "ui_config.h"
#include "defines.h"
#include "languageselectdialog.h"

Application::Application(int argc, char *argv[]) :
    QApplication(argc, argv)
    , _configWidget(0)
    , _trayIcon(0)
    , _trayIconMenu(0)
    //, _shortcutScreenFull(0)
    //, _shortcutScreenPart(0)
    //, _shortcutTextShare(0)
    , _network(0)
    , _settings(0)
{
}

Application::~Application()
{
    _trayIcon->hide();
    _settings->sync();
    //delete _shortcutScreenPart;
    //delete _shortcutScreenFull;
    //delete _shortcutTextShare;
    delete _trayIconMenu;
}

bool Application::pxAppInit()
{
    QLocalSocket socket;
    socket.connectToServer(APP_NAME);
    if (socket.waitForConnected(500)) {
        qDebug() << "Application allready launched!";
        return false;
    }
    _localServer = new QLocalServer(this);

    if (!_localServer->listen(APP_NAME)) {
        QLocalServer::removeServer(APP_NAME);
        _localServer->listen(APP_NAME);
    }

    QString homePath = QDir::homePath();
    QString settingsFile = homePath + "/" + SETTINGS_FILE;
    _settings = new QSettings(settingsFile, QSettings::IniFormat, this);

    initLanguages();

    _configWidget = new ConfigWidget(_settings, _languages);
    QObject::connect(_configWidget, SIGNAL(settingsChanged()), this, SLOT(setupHotkeys()));

    _configWidget->init();

    //_shortcutScreenFull = new QxtGlobalShortcut;
    //_shortcutScreenPart = new QxtGlobalShortcut;
    //_shortcutTextShare = new QxtGlobalShortcut;

    connectDisconectHotkeys(true);
    connect(_configWidget, SIGNAL(showSignal(bool)), this, SLOT(connectDisconectHotkeys(bool)));

    _trayIconMenu = new QMenu;
    _trayIconMenu->addAction(tr("About"), this, SLOT(aboutDialog()));
    _trayIconMenu->addAction("", this, SLOT(processCodeShare()));
    _trayIconMenu->addAction("", this, SLOT(processScreenshotFull()));
    _trayIconMenu->addAction("", this, SLOT(processScreenshotPart()));
    _trayIconMenu->addAction(tr("Configure"), _configWidget, SLOT(show()));
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(tr("Exit"), this, SLOT(quit()));                   // Tray menu

    setupHotkeys();

    _trayIcon = new QSystemTrayIcon(QIcon(":/icons/icon.png"), this);
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    _trayIcon->setContextMenu(_trayIconMenu);                                   // Tray icon

    _network = new Network(_settings, this);
    connect(_network, SIGNAL(linkReceived(QString)), SLOT(linkAvaliable(QString)));     // Network

    this->setQuitOnLastWindowClosed(false);

    _trayIcon->show();
    QFile file(settingsFile);

    if (!file.exists())
        _configWidget->show();
    return true;
}

void Application::newLocalSocketConnection()
{
}

void Application::connectDisconectHotkeys(bool b)
{
    static bool alreadyConnect = false;

    if (!alreadyConnect && b) {
        //connect(_shortcutScreenFull, SIGNAL(activated()), this, SLOT(processScreenshotFull()));
        //connect(_shortcutScreenPart, SIGNAL(activated()), this, SLOT(processScreenshotPart()));
        //connect(_shortcutTextShare, SIGNAL(activated()), this, SLOT(processCodeShare()));
    }

    if (!b){
        //disconnect(_shortcutScreenFull, SIGNAL(activated()), this, SLOT(processScreenshotFull()));
        //disconnect(_shortcutScreenPart, SIGNAL(activated()), this, SLOT(processScreenshotPart()));
        //disconnect(_shortcutTextShare, SIGNAL(activated()), this, SLOT(processCodeShare()));
    }

    alreadyConnect = b;
}

void Application::processScreenshot(bool isFullScreen)
{
    QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(0);
    if (!isFullScreen) {
        ImageSelectWidget imageSelectDialog(&pixmap);
        imageSelectDialog.setWindowState(Qt::WindowFullScreen);
        if (!imageSelectDialog.exec())
            return;
    }

    QString imagetype = _settings->value("general/imagetype", DEFAULT_IMAGE_TYPE).toString();

    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QFile::WriteOnly);
    pixmap.save(&buffer, imagetype.toLocal8Bit().constData());
    buffer.close();
    _network->upload(imageBytes, imagetype);
}

void Application::processCodeShare()
{
    bool showsourcedialog = _settings->value("general/showsourcedialog", DEFAULT_SHOW_SOURCES_CONF_DIALOG).toBool();
    if (showsourcedialog) {
        LanguageSelectDialog dialog(_settings, _languages);
        if (!dialog.exec()) {
            return;
        }
    }

    QString sourcestype = _settings->value("general/sourcetype", DEFAULT_SOURCES_TYPE).toString();
    QString text = QApplication::clipboard()->text();
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
    QApplication::clipboard()->setText(link);
    _trayIcon->showMessage(tr("Done!"), tr("File shared!"), QSystemTrayIcon::Information, 6500);
}

void Application::aboutDialog()
{
    QMessageBox::information(NULL, APP_NAME,
                             tr("Utility for easy screenshoting and code sharing<br> We live at <a href=\"http://pastexen.com/\">pastexen.com</a><br> Hosting provided by <a href=\"http://scalaxy.ru/\">scalaxy.ru</a>"));
}

void Application::setupHotkeys()
{
    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    QString codeHotkey = _settings->value("general/texthotkey", DEFAULT_HOTKEY_CODE).toString();

    //#ifdef Q_OS_WIN
    //if (!RegisterHotKey(NULL, 1, MOD_ALT, 0x42)) {
    //    qDebug() << "Error activating hothey!";
    //}
    //#endif

    //if (!_shortcutScreenFull->setShortcut(QKeySequence(fullHotkey)))
    //    qDebug() << "Error activating hotkey:" << fullHotkey;          // Shortcut for full screen

    //if (!_shortcutScreenPart->setShortcut(QKeySequence(partHotkey)))
   //     qDebug() << "Error activating hotkey:" << partHotkey;        // Shortcut for part of the screen

   // if (!_shortcutTextShare->setShortcut(QKeySequence(codeHotkey)))
   //     qDebug() << "Error activating hotkey:" << codeHotkey;          // Shortcut for text share

    QList<QAction*> actsList = _trayIconMenu->actions();
    actsList[1]->setText(tr("Text share (%1)").arg(codeHotkey));
    actsList[2]->setText(tr("Full s-shot (%1)").arg(fullHotkey));
    actsList[3]->setText(tr("Half s-shot (%1)").arg(partHotkey));
}

void Application::initLanguages()
{
    _languages.insert("txt", "Plain text");
    _languages.insert("c", "C");
    _languages.insert("cpp", "C++");
    _languages.insert("java", "Java");
    _languages.insert("php", "PHP");
    _languages.insert("py", "Python");
    _languages.insert("pl", "Perl");
    _languages.insert("pas", "Pascal");
    _languages.insert("m", "ObjectiveC");
    _languages.insert("xml", "XML");
    _languages.insert("html", "HTML");
    _languages.insert("js", "Javascript");
    _languages.insert("css", "CSS");
    _languages.insert("json", "Json");
    _languages.insert("as", "ActionScript");
    _languages.insert("vb", "VBscript");
    _languages.insert("d", "D");
    _languages.insert("sql", "SQL");
    _languages.insert("st", "Smalltalk");
    _languages.insert("lisp", "LISP");
    _languages.insert("ini", "ini");
    _languages.insert("conf", "Apache");
    _languages.insert("sh", "BASH");
    _languages.insert("bat", "Dos (bat)");
    _languages.insert("cmake", "CMake");
    _languages.insert("hs", "Haskell");
}
