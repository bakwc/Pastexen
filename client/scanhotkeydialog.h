#ifndef SCANHOTKEYDIALOG_H
#define SCANHOTKEYDIALOG_H

#include <QDialog>
#include <QKeySequence>
#include <QKeyEvent>
#include "ui_scanHotkeysDialog.h"

class ScanHotkeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScanHotkeyDialog(QWidget *parent = 0);
    const QString& key() { return _key; }

protected:
    bool event(QEvent *);
    
signals:
    
public slots:

private:
    Ui::Dialog _ui;
    QString _key;
    QString rigthStringKey(const QString& key);
    
};

#endif // SCANHOTKEYDIALOG_H
