#pragma once

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QMap>
#include "ui_config.h"

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    ConfigWidget(QSettings *settings, QMap<QString, QString> &languages, QWidget *parent = 0);
    void init();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
#if defined(Q_OS_WIN)
    bool winEvent (MSG * message, long * result);
#elif defined(Q_OS_LINUX)

#endif
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
signals:
    void settingsChanged();
    void showSignal(bool b);

public slots:
    void applyChanges();
    void changeHotkey();
private:
    void showTypes(QString fullHotkey, QString partHotkey, QString textHotkey);
    void registerActualHotkeys();
    Ui::ConfigForm _ui;
    QSettings *_settings;
    QMap<QString, QString> &_languages;

#if defined(Q_OS_WIN)
    size_t qtKeyToWin(size_t key);
    void registerHotkeyWin(const QString& str, size_t hotkeyId);
#endif
};
