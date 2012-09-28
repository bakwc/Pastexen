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
    ConfigWidget(Ui::Form *ui, QSettings *settings, QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    void init();
public slots:
    void applyChanges();
private:
    Ui::Form *_ui;
    QSettings *_settings;
};

#endif // CONFIGWIDGET_H