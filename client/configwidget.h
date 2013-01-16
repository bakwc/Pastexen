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
    bool winEvent (MSG * message, long * result);
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
    void registerHotkeyWin(const QString& str, size_t hotkeyId);
    size_t qtKeyToWin(size_t key);
    Ui::ConfigForm _ui;
    QSettings *_settings;
    QMap<QString, QString> &_languages;
};
