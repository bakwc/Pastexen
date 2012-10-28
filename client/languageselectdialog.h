#pragma once

#include <QDialog>
#include <QSettings>
#include <QMap>

namespace Ui {
class LanguageSelectDialog;
}

class LanguageSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LanguageSelectDialog(QSettings *settings, QMap<QString, QString> &languages, QWidget *parent = 0);
    ~LanguageSelectDialog();
    void showTypes();
    
private slots:
    void on_buttonBox_accepted();

private:
    QSettings *_settings;
    QMap<QString, QString> &_languages;
    Ui::LanguageSelectDialog *ui;
};
