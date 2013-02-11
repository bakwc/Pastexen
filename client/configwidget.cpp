#include "configwidget.h"
#include <QCloseEvent>
#include <QHideEvent>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QDebug>

#include "application.h"
#include "scanhotkeydialog.h"
#include "defines.h"

ConfigWidget::ConfigWidget(QSettings *settings, QMap<QString, QString> &languages, QWidget *parent)
    : QWidget(parent),
      _settings(settings),
      _languages(languages),
      _hotKeys(new UGlobalHotkeys(parent))
{
    _ui.setupUi(this);

    this->setGeometry(QDesktopWidget().availableGeometry().center().x() - (this->width() / 2),
                      QDesktopWidget().availableGeometry().center().y() - (this->height() / 2),
                       this->width(), this->height());
    this->setFixedSize(this->size());

    connect(_ui.cancelButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(_ui.applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));    // Config window

    connect(_ui.fullhotkey, SIGNAL(clicked()), this, SLOT(changeHotkey()));
    connect(_ui.parthotkey, SIGNAL(clicked()), this, SLOT(changeHotkey()));
    connect(_ui.texthotkey, SIGNAL(clicked()), this, SLOT(changeHotkey()));
    connect(_hotKeys, SIGNAL(Activated(size_t)), this, SIGNAL(hotkeyActivated(size_t)));
    registerActualHotkeys();
}

void ConfigWidget::registerActualHotkeys() {
    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    QString codeHotkey = _settings->value("general/texthotkey", DEFAULT_HOTKEY_CODE).toString();

    _hotKeys->RegisterHotkey(partHotkey, HOTKEY_PART_ID);
    _hotKeys->RegisterHotkey(fullHotkey, HOTKEY_FULL_ID);
    _hotKeys->RegisterHotkey(codeHotkey, HOTKEY_CODE_ID);
}

void ConfigWidget::init()
{
    this->setWindowTitle(QString("%1 - %2")
                         .arg(APP_NAME)
                         .arg(tr("Config")));

    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    QString codeHotkey = _settings->value("general/texthotkey", DEFAULT_HOTKEY_CODE).toString();

    showTypes(fullHotkey, partHotkey, codeHotkey);

    QString imagetype = _settings->value("general/imagetype", DEFAULT_IMAGE_TYPE).toString();
    QString sourcestype = _settings->value("general/sourcetype", DEFAULT_SOURCES_TYPE).toString();
    bool showsourcedialog = _settings->value("general/showsourcedialog", DEFAULT_SHOW_SOURCES_CONF_DIALOG).toBool();

    int imgIndex = _ui.comboImageType->findData(imagetype);
    if (imgIndex != -1) {
        _ui.comboImageType->setCurrentIndex(imgIndex);
    }

    int srcIndex = _ui.comboSourcesType->findData(sourcestype);
    if (srcIndex != -1) {
        _ui.comboSourcesType->setCurrentIndex(srcIndex);
    }

    QString uuid = _settings->value("general/uuid").toString();
    QString regText = tr("<a href=\"http://pastexen.com/register.php?uuid=%1\">Register & attach client</a>")
            .arg(uuid);
    _ui.regLabel->setText(regText);
    qDebug() << regText;
    _ui.checkBoxLangDialogShow->setChecked(showsourcedialog);
}

void ConfigWidget::showTypes(QString fullHotkey, QString partHotkey, QString textHotkey)
{
    _ui.comboImageType->addItem("JPG", QString("jpg"));
    _ui.comboImageType->addItem("PNG", QString("png"));

    for (QMap<QString, QString>::iterator i = _languages.begin(); i != _languages.end(); i++) {
        _ui.comboSourcesType->addItem(i.value(), i.key());
    }

    _ui.fullhotkey->setText(fullHotkey);
    _ui.parthotkey->setText(partHotkey);
    _ui.texthotkey->setText(textHotkey);
}


void ConfigWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void ConfigWidget::showEvent(QShowEvent *event)
{
    emit showSignal(false);
    raise();
    activateWindow();
    QWidget::showEvent(event);
}

void ConfigWidget::hideEvent(QHideEvent *event)
{
    emit showSignal(true);
    QWidget::hideEvent(event);
}

void ConfigWidget::applyChanges()
{
    emit settingsChanged();
    _settings->setValue("general/imagetype", _ui.comboImageType->itemData(_ui.comboImageType->currentIndex()).toString());
    _settings->setValue("general/sourcetype", _ui.comboSourcesType->itemData(_ui.comboSourcesType->currentIndex()).toString());
    _settings->setValue("general/showsourcedialog", _ui.checkBoxLangDialogShow->isChecked());
    _settings->sync();
    this->hide();
}

void ConfigWidget::changeHotkey()
{
    ScanHotkeyDialog dial(this);
    dial.setModal(true);
    if (dial.exec()) {
        QPushButton* b = qobject_cast<QPushButton*>(sender());
        if (b) {
            QString settingsKey("general/");
            settingsKey += b->objectName();

            _settings->setValue(settingsKey, dial.key());

            b->setText(dial.key());

            registerActualHotkeys();
        }
    }
}
