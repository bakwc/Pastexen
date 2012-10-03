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

    /*auto login = _settings->value("general/login", DEFAULT_LOGIN).toString();
    auto pass = _settings->value("general/password", DEFAULT_PASS).toString();
    auto dir = _settings->value("general/directory", DEFAULT_DIR).toString();
    auto publish = _settings->value("general/publish", DEFAULT_PUBLISH).toBool();

    _ui->editLogin->setText(login);
    _ui->editPass->setText(pass);
    _ui->editDir->setText(dir);
    _ui->checkBoxPublish->setChecked(publish);*/
}


void ConfigWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void ConfigWidget::applyChanges()
{
    _settings->setValue("general/imagetype", _ui->comboImageType->itemData(_ui->comboImageType->currentIndex()).toString());
    _settings->setValue("general/sourcetype", _ui->comboSourcesType->itemData(_ui->comboSourcesType->currentIndex()).toString());
    _settings->setValue("general/showsourcedialog", _ui->checkBoxLangDialogShow->isChecked());
    _settings->sync();
    this->hide();
}
