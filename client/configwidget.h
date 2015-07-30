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
    ConfigWidget(const QString& appName, QMap<QString, QString> &languages, QWidget *parent = 0);
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
private slots:
    void on_pushButton_clicked();

private:
    void showTypes(QString fullHotkey, QString partHotkey, QString textHotkey);
    void registerActualHotkeys();
    Ui::ConfigForm _ui;
    QString _appName;
    QMap<QString, QString> &_languages;
    UGlobalHotkeys* _hotKeys;
};
