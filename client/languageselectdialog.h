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
    explicit LanguageSelectDialog(QMap<QString, QString> &languages, QWidget *parent = 0);
    ~LanguageSelectDialog();
    void showTypes();
    
private slots:
    void on_buttonBox_accepted();

private:
    QMap<QString, QString> &_languages;
    Ui::LanguageSelectDialog *ui;
};
