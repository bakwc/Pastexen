#include "configwidget.h"
#include <QCloseEvent>
#include "defines.h"

ConfigWidget::ConfigWidget(Ui::Form *ui, QSettings *settings, QWidget *parent)
    : QWidget(parent),
      _ui(ui),
      _settings(settings)
{
}

void ConfigWidget::init()
{
    this->setWindowTitle(QString("%1 - %2")
                         .arg(APP_NAME)
                         .arg(tr("Config")));

    QIcon icon(":/icons/yascreens.png");
    this->setWindowIcon(icon);

    auto login = _settings->value("general/login", DEFAULT_LOGIN).toString();
    auto pass = _settings->value("general/password", DEFAULT_PASS).toString();
    auto dir = _settings->value("general/directory", DEFAULT_DIR).toString();
    auto publish = _settings->value("general/publish", DEFAULT_PUBLISH).toBool();

    _ui->editLogin->setText(login);
    _ui->editPass->setText(pass);
    _ui->editDir->setText(dir);
    _ui->checkBoxPublish->setChecked(publish);
}


void ConfigWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void ConfigWidget::applyChanges()
{
    QString dir=_ui->editDir->text();
    if (dir.size()==0)
        dir="/";
    if (dir[0]!='/')
        dir="/"+dir;
    if (dir[dir.size()-1]!='/')
        dir=dir+"/";
    _ui->editDir->setText(dir);

    _settings->setValue("general/login", _ui->editLogin->text());
    _settings->setValue("general/password", _ui->editPass->text());
    _settings->setValue("general/directory", dir);
    _settings->setValue("general/publish", _ui->checkBoxPublish->isChecked());
    _settings->sync();
    this->hide();
}
