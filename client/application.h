#ifndef APPLICATION_H
#define APPLICATION_H

#include <QxtGui/QxtApplication>
#include <QxtGui/QxtGlobalShortcut>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QFile>
#include "configwidget.h"
#include "network.h"

class Application : public QxtApplication
{
    Q_OBJECT
public:
    Application(int argc, char *argv[]);
    ~Application();
    void init(const QFile& file);
private slots:
    inline void processScreenshotFull() { processScreenshot(true); }
    inline void processScreenshotPart() { processScreenshot(false); }
    void trayIconClicked(const QSystemTrayIcon::ActivationReason &button);
    void linkAvaliable(const QString &link);
    void aboutDialog();
private:
    void processScreenshot(bool isFullScreen);
private:
    ConfigWidget *_configWidget;
    QSystemTrayIcon *_trayIcon;
    QMenu *_trayIconMenu;
    QxtGlobalShortcut *_shortcutScreenFull;
    QxtGlobalShortcut *_shortcutScreenPart;
    Network *_network;
    Ui::Form *_configUi;
    QSettings *_settings;
};

#endif // APPLICATION_H
