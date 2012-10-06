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

    showTypes();

    auto imagetype = _settings->value("general/imagetype", DEFAULT_IMAGE_TYPE).toString();
    auto sourcestype = _settings->value("general/sourcetype", DEFAULT_SOURCES_TYPE).toString();
    bool showsourcedialog = _settings->value("general/showsourcedialog", DEFAULT_SHOW_SOURCES_CONF_DIALOG).toBool();

    int imgIndex = _ui->comboImageType->findData(imagetype);
    if (imgIndex != -1) {
        _ui->comboImageType->setCurrentIndex(imgIndex);
    }

    int srcIndex = _ui->comboSourcesType->findData(sourcestype);
    if (srcIndex != -1) {
        _ui->comboSourcesType->setCurrentIndex(srcIndex);
    }

    _ui->checkBoxLangDialogShow->setChecked(showsourcedialog);
}

void ConfigWidget::showTypes()
{
    _ui->comboImageType->addItem("JPG", QString("jpg"));
    _ui->comboImageType->addItem("PNG", QString("png"));

    _ui->comboSourcesType->addItem(tr("Plain text"), QString("txt"));
    _ui->comboSourcesType->addItem(tr("C++"), QString("cpp"));
    _ui->comboSourcesType->addItem(tr("Pascal"), QString("pas"));
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
