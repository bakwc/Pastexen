#pragma once

#include <QSystemTrayIcon>
#include <QMenu>
#include <QMap>
#include <QSettings>
#include <QFile>
#include <QLocalServer>
#include "configwidget.h"
#include "network.h"

#ifdef Q_OS_WIN
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x500
#include <windows.h>
#endif

#ifdef Q_OS_LINUX
#include <X11/extensions/XTest.h>
#define KEY_DOWN True
#define KEY_UP   False
#define KEYCODE_LCONTROL 37
#define KEYCODE_C 54
#endif


class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char *argv[]);
    ~Application();
    bool pxAppInit();
public slots:
    inline void hotkeyPressed(size_t id) {
        if (id == HOTKEY_CODE_ID) {
            processCodeShare();
        } else if (id == HOTKEY_FULL_ID) {
            processScreenshot(true);
        } else if (id == HOTKEY_PART_ID) {
            processScreenshot(false);
        }
    }
    inline void processScreenshotFull() { processScreenshot(true); }
    inline void processScreenshotPart() { processScreenshot(false); }
    void processCodeShare();
private slots:
    void trayIconClicked(const QSystemTrayIcon::ActivationReason &button);
    void linkAvaliable(const QString &link);
    void aboutDialog();
    void setupHotkeys();
    void newLocalSocketConnection();
    void trayMessage(const QString &caption, const QString &text);
private:
    void processScreenshot(bool isFullScreen);
    void initLanguages();
    bool checkEllapsed();
    void timerEvent(QTimerEvent *);
private:
    ConfigWidget *_configWidget;
    QSystemTrayIcon *_trayIcon;
    QMenu *_trayIconMenu;
    Network *_network;
    QSettings *_settings;
    QMap<QString, QString> _languages;
    QLocalServer *_localServer;
    QTime _lastSended;
    bool Sharing = false;
    int _timerId;
};
