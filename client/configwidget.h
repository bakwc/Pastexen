#pragma once

#include <QWidget>
#include <QString>
#include <QSettings>
#include <QMap>

#include "../utils/uglobalhotkeys.h"

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
signals:
    void settingsChanged();
    void showSignal(bool b);
    void hotkeyActivated(size_t id);
public slots:
    void applyChanges();
    void changeHotkey();
private:
    void showTypes(QString fullHotkey, QString partHotkey, QString textHotkey);
    void registerActualHotkeys();
    Ui::ConfigForm _ui;
    QSettings *_settings;
    QMap<QString, QString> &_languages;
    UGlobalHotkeys* _hotKeys;
};
