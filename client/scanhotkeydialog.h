#pragma once

#include <QDialog>
#include <QKeySequence>
#include <QKeyEvent>
#include "ui_scanHotkeysDialog.h"

#include "../utils/ukeysequence.h"

class ScanHotkeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScanHotkeyDialog(QWidget *parent = 0);
    QString key() { return Keys.ToString(); }

protected:
    bool event(QEvent *);
    
signals:
    
public slots:

private:
    Ui::Dialog _ui;
    UKeySequence Keys;
};
