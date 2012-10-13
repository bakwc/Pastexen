#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>
#include <QString>
#include <QSettings>
#include "ui_config.h"

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    ConfigWidget(QSettings *settings, QWidget *parent = 0);
    ~ConfigWidget();
    void closeEvent(QCloseEvent *event);
    void init(QString fullHotkey, QString partHotkey, QString textHotkey);
public slots:
    void applyChanges();
private:
    void showTypes(QString fullHotkey, QString partHotkey, QString textHotkey);
private:
    Ui::ConfigForm *_ui;
    QSettings *_settings;
};

#endif // CONFIGWIDGET_H
