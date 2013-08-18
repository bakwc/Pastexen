#include "configwidget.h"
#include <QCloseEvent>
#include <QHideEvent>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QDebug>

#include "application.h"
#include "scanhotkeydialog.h"
#include "defines.h"

ConfigWidget::ConfigWidget(const QString& appName,
                           QMap<QString, QString> &languages,
                           QWidget *parent)
    : QWidget(parent),
      _appName(appName),
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
    QString fullHotkey = Application::settings().GetParameter("general/fullhotkey", DEFAULT_HOTKEY_FULL);
    QString partHotkey = Application::settings().GetParameter("general/parthotkey", DEFAULT_HOTKEY_PART);
    QString codeHotkey = Application::settings().GetParameter("general/texthotkey", DEFAULT_HOTKEY_CODE);

    _hotKeys->RegisterHotkey(partHotkey, HOTKEY_PART_ID);
    _hotKeys->RegisterHotkey(fullHotkey, HOTKEY_FULL_ID);
    _hotKeys->RegisterHotkey(codeHotkey, HOTKEY_CODE_ID);
}

void ConfigWidget::init()
{
    this->setWindowTitle(QString("%1 - %2")
                         .arg(_appName)
                         .arg(tr("Config")));

    QString fullHotkey = Application::settings().GetParameter("general/fullhotkey", DEFAULT_HOTKEY_FULL);
    QString partHotkey = Application::settings().GetParameter("general/parthotkey", DEFAULT_HOTKEY_PART);
    QString codeHotkey = Application::settings().GetParameter("general/texthotkey", DEFAULT_HOTKEY_CODE);

    showTypes(fullHotkey, partHotkey, codeHotkey);

    QString imagetype = Application::settings().GetParameter("general/imagetype", DEFAULT_IMAGE_TYPE);
    QString sourcestype = Application::settings().GetParameter("general/sourcetype", DEFAULT_SOURCES_TYPE);
    bool showsourcedialog = Application::settings().GetParameter("general/showsourcedialog", ToString(DEFAULT_SHOW_SOURCES_CONF_DIALOG));

    int imgIndex = _ui.comboImageType->findData(imagetype);
    if (imgIndex != -1) {
        _ui.comboImageType->setCurrentIndex(imgIndex);
    }

    int srcIndex = _ui.comboSourcesType->findData(sourcestype);
    if (srcIndex != -1) {
        _ui.comboSourcesType->setCurrentIndex(srcIndex);
    }

    QString regText = tr("<a href=\"%1\">My account</a>"
                         " (you need to share file first)")
            .arg(Application::GetAccountUrl());
    _ui.regLabel->setText(regText);
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
    Application::settings().SetParameter("general/imagetype", _ui.comboImageType->itemData(_ui.comboImageType->currentIndex()).toString());
    Application::settings().SetParameter("general/sourcetype", _ui.comboSourcesType->itemData(_ui.comboSourcesType->currentIndex()).toString());
    Application::settings().SetParameter("general/showsourcedialog", ToString(_ui.checkBoxLangDialogShow->isChecked()));
    Application::settings().Save();
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

            Application::settings().SetParameter(settingsKey, dial.key());

            b->setText(dial.key());

            registerActualHotkeys();
        }
    }
}
