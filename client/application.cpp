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
#include <QThread>
#include "application.h"
#include "imageselectwidget.h"
#include "ui_config.h"
#include "defines.h"
#include "languageselectdialog.h"

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <string>
#include <climits>

QString getClipboardText() {
    std::string result;
    Display* d = XOpenDisplay(nullptr);
    if(d) {
        Window wParent = DefaultRootWindow(d);
        Window w = XCreateSimpleWindow(
            d, wParent, 0, 0, 1, 1, 0, CopyFromParent, CopyFromParent);
        Atom target = XInternAtom(d, "UTF8_STRING", False);
        XConvertSelection(d, XA_PRIMARY, target, None, w, CurrentTime);
        XFlush(d);

        for(unsigned long offset = 0;;) {
            XEvent ev;
            XNextEvent(d, &ev);
            if(SelectionNotify == ev.type
               && None != ev.xselection.property
            ) {
                Atom actual_type_return = None;
                int actual_format_return = 0;
                unsigned long nitems_return = 0;
                unsigned long bytes_after_return = 0;
                unsigned char* prop_return = nullptr;
                XGetWindowProperty(
                    d, w, ev.xselection.property, offset, INT_MAX
                    , False, AnyPropertyType, &actual_type_return
                    , &actual_format_return, &nitems_return
                    , &bytes_after_return, &prop_return);
                if(nitems_return) {
                    result.append(reinterpret_cast<const char*>(prop_return)
                                  , nitems_return);
                    offset += nitems_return;
                }
                XDeleteProperty(d, w, ev.xselection.property);
                if(!bytes_after_return) {
                    break;
                }
            }
        }
        XDestroyWindow(d, w);
        XCloseDisplay(d);
    }
    return QString(result.c_str());
}
#endif

Application::Application(int argc, char *argv[]) :
    QApplication(argc, argv)
    , _configWidget(0)
    , _trayIcon(0)
    , _trayIconMenu(0)
    , _network(0)
    , _settings(0)
{
}

Application::~Application()
{
    _trayIcon->hide();
    _settings->sync();
    delete _trayIconMenu;
}

bool Application::pxAppInit()
{
    QLocalSocket socket;
    socket.connectToServer(APP_NAME);
    if (socket.waitForConnected(500)) {
        qDebug() << "Application already launched!";
        return false;
    }

    _lastSended.start();

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
    connect(_configWidget, SIGNAL(settingsChanged()), SLOT(setupHotkeys()));
    connect(_configWidget, SIGNAL(hotkeyActivated(size_t)), SLOT(hotkeyPressed(size_t)));

    _configWidget->init();

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
    connect(_network, SIGNAL(trayMessage(QString,QString)), SLOT(trayMessage(QString,QString)));

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

void Application::trayMessage(const QString &caption, const QString &text)
{
    _trayIcon->showMessage(caption, text, QSystemTrayIcon::Information, 6500);
}

void Application::processScreenshot(bool isFullScreen)
{
    if (Sharing) {
        return;
    }
    if (!checkEllapsed()) {
        return;
    }
    Sharing = true;
    QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(0);
    if (!isFullScreen) {
        ImageSelectWidget imageSelectDialog(&pixmap);
        imageSelectDialog.setWindowState(Qt::WindowFullScreen);
        if (!imageSelectDialog.exec()) {
            Sharing = false;
            return;
        }
    }

    QString imagetype = _settings->value("general/imagetype", DEFAULT_IMAGE_TYPE).toString();

    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QFile::WriteOnly);
    pixmap.save(&buffer, imagetype.toLocal8Bit().constData());
    buffer.close();
    _network->upload(imageBytes, imagetype);
    Sharing = false;
}

void Application::processCodeShare()
{
    if (!checkEllapsed()) {
        return;
    }

    bool showsourcedialog = _settings->value("general/showsourcedialog", DEFAULT_SHOW_SOURCES_CONF_DIALOG).toBool();
    if (showsourcedialog) {
        LanguageSelectDialog dialog(_settings, _languages);
        if (!dialog.exec()) {
            return;
        }
    }

    QString sourcestype = _settings->value("general/sourcetype", DEFAULT_SOURCES_TYPE).toString();

#ifdef Q_OS_WIN
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    QThread::msleep(100);
    ip.ki.wVk = 'C';
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    QThread::msleep(100);
    ip.ki.wVk = 'C';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    QThread::msleep(100);
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    QThread::msleep(100);
    #endif

    QString text;
#ifdef Q_OS_LINUX
        Display *dpy = XOpenDisplay(NULL);
        if (!dpy) qDebug() << "DPU ERROR!";
        XTestFakeKeyEvent(dpy, KEYCODE_LCONTROL, KEY_DOWN, CurrentTime);
        XTestFakeKeyEvent(dpy, KEYCODE_C, KEY_DOWN, 0);
        QThread::msleep(200);
        XTestFakeKeyEvent(dpy, KEYCODE_LCONTROL, KEY_UP, CurrentTime);
        XTestFakeKeyEvent(dpy, KEYCODE_C, KEY_UP, 0);
        XCloseDisplay(dpy);
        text = getClipboardText();
#elif defined(Q_OS_WIN)
    text = QApplication::clipboard()->text();
#endif


    if (text.count() == 0) {
        _trayIcon->showMessage(tr("Error!"), tr("No text found in clipboard"), QSystemTrayIcon::Information, 6500);
        return;
    }
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

    QList<QAction*> actsList = _trayIconMenu->actions();
    actsList[1]->setText(tr("Text share (%1)").arg(codeHotkey));
    actsList[2]->setText(tr("Full s-shot (%1)").arg(fullHotkey));
    actsList[3]->setText(tr("Half s-shot (%1)").arg(partHotkey));
}

void Application::initLanguages()
{
    _languages.insert("auto", tr("Auto detection"));
    _languages.insert("txt", tr("Plain text"));
    _languages.insert("c", "C");
    _languages.insert("cpp", "C++");
    _languages.insert("cs", "C#");
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

bool Application::checkEllapsed()
{
    if (_lastSended.elapsed() < 3000) {
        return false;
    }
    _lastSended.restart();
    return true;
}
