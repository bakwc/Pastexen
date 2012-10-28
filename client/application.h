#pragma once

#include <QxtGui/QxtApplication>
#include <QxtGui/QxtGlobalShortcut>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMap>
#include <QSettings>
#include <QFile>
#include <QLocalServer>
#include "configwidget.h"
#include "network.h"

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char *argv[]);
    ~Application();
    bool pxAppInit();
private slots:
    inline void processScreenshotFull() { processScreenshot(true); }
    inline void processScreenshotPart() { processScreenshot(false); }
    void processCodeShare();
    void trayIconClicked(const QSystemTrayIcon::ActivationReason &button);
    void linkAvaliable(const QString &link);
    void aboutDialog();
    void setupHotkeys();
    void newLocalSocketConnection();
    void connectDisconectHotkeys(bool b);
private:
    void processScreenshot(bool isFullScreen);
    void initLanguages();
private:
    ConfigWidget *_configWidget;
    QSystemTrayIcon *_trayIcon;
    QMenu *_trayIconMenu;
    QxtGlobalShortcut *_shortcutScreenFull;
    QxtGlobalShortcut *_shortcutScreenPart;
    QxtGlobalShortcut *_shortcutTextShare;
    Network *_network;
    QSettings *_settings;
    QMap<QString, QString> _languages;
    QLocalServer *_localServer;
};
