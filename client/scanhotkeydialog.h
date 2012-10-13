#ifndef SCANHOTKEYDIALOG_H
#define SCANHOTKEYDIALOG_H

#include <QDialog>
#include <QKeySequence>

class ScanHotkeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScanHotkeyDialog(QWidget *parent = 0);
    const QKeySequence& key() { return _key; }
    
signals:
    
public slots:

private:
    QKeySequence _key;
    
};

#endif // SCANHOTKEYDIALOG_H
