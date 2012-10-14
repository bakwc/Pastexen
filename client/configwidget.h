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
    void closeEvent(QCloseEvent *event);
    void init(QString fullHotkey, QString partHotkey, QString textHotkey);

signals:
    void settingsChanged();

public slots:
    void applyChanges();
    void changeHotkey();
private:
    void showTypes(QString fullHotkey, QString partHotkey, QString textHotkey);

    Ui::ConfigForm _ui;
    QSettings *_settings;
};
