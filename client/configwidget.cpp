#include "configwidget.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include "scanhotkeydialog.h"
#include "defines.h"
#include <QDebug>

ConfigWidget::ConfigWidget(QSettings *settings, QWidget *parent)
    : QWidget(parent),
      _ui(new Ui::ConfigForm),
      _settings(settings)
{
    _ui->setupUi(this);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setGeometry(QDesktopWidget().availableGeometry().center().x() - (this->width() / 2),
                      QDesktopWidget().availableGeometry().center().y() - (this->height() / 2),
                       this->width(), this->height());

    connect(_ui->cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(_ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));    // Config window

    connect(_ui->screenFullButton, SIGNAL(clicked()), this, SLOT(changeHotkey()));
    connect(_ui->screenPartButton, SIGNAL(clicked()), this, SLOT(changeHotkey()));
    connect(_ui->shareTextButton, SIGNAL(clicked()), this, SLOT(changeHotkey()));
}

ConfigWidget::~ConfigWidget()
{
    delete _ui;
}

void ConfigWidget::init(QString fullHotkey, QString partHotkey, QString textHotkey)
{
    this->setWindowTitle(QString("%1 - %2")
                         .arg(APP_NAME)
                         .arg(tr("Config")));

    QIcon icon(":/icons/icon.png");
    this->setWindowIcon(icon);

    showTypes(fullHotkey, partHotkey, textHotkey);

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

void ConfigWidget::showTypes(QString fullHotkey, QString partHotkey, QString textHotkey)
{
    _ui->comboImageType->addItem("JPG", QString("jpg"));
    _ui->comboImageType->addItem("PNG", QString("png"));

    _ui->comboSourcesType->addItem(tr("Plain text"), QString("txt"));
    _ui->comboSourcesType->addItem(tr("C++"), QString("cpp"));
    _ui->comboSourcesType->addItem(tr("Pascal"), QString("pas"));

    _ui->screenFullButton->setText(fullHotkey);
    _ui->screenPartButton->setText(partHotkey);
    _ui->shareTextButton->setText(textHotkey);
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

void ConfigWidget::changeHotkey()
{
    ScanHotkeyDialog dial(this);
    dial.setModal(true);
    if (dial.exec())
        qDebug() << "ScanHotkeyDialog return true";
    else
        qDebug() << "ScanHotkeyDeialog return false";
}
