#pragma once

#include <QWidget>
#include <QString>
#include <QSettings>
#include "ui_config.h"

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    ConfigWidget(QSettings *settings, QWidget *parent = 0);
    void init(QString fullHotkey, QString partHotkey, QString textHotkey);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

signals:
    void settingsChanged();
    void showSignal(bool b);

public slots:
    void applyChanges();
    void changeHotkey();
private:
    void showTypes(QString fullHotkey, QString partHotkey, QString textHotkey);

    Ui::ConfigForm _ui;
    QSettings *_settings;
};
