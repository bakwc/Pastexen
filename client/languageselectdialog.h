#pragma once

#include <QDialog>
#include <QSettings>

namespace Ui {
class LanguageSelectDialog;
}

class LanguageSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LanguageSelectDialog(QSettings *settings, QWidget *parent = 0);
    ~LanguageSelectDialog();
    void showTypes();
    
private slots:
    void on_buttonBox_accepted();

private:
    QSettings *_settings;
    Ui::LanguageSelectDialog *ui;
};
