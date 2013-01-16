#include "configwidget.h"
#include <QCloseEvent>
#include <QHideEvent>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QDebug>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include "application.h"
#include "scanhotkeydialog.h"
#include "defines.h"

ConfigWidget::ConfigWidget(QSettings *settings, QMap<QString, QString> &languages, QWidget *parent)
    : QWidget(parent),
      _settings(settings),
      _languages(languages)
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
    registerActualHotkeys();
}

void ConfigWidget::registerActualHotkeys() {
    QString fullHotkey = _settings->value("general/fullhotkey", DEFAULT_HOTKEY_FULL).toString();
    QString partHotkey = _settings->value("general/parthotkey", DEFAULT_HOTKEY_PART).toString();
    QString codeHotkey = _settings->value("general/texthotkey", DEFAULT_HOTKEY_CODE).toString();

    qDebug() << "Full hotkey: " << fullHotkey;

    #ifdef Q_OS_WIN
    registerHotkeyWin(partHotkey, HOTKEY_PART_ID);
    registerHotkeyWin(fullHotkey, HOTKEY_FULL_ID);
    registerHotkeyWin(codeHotkey, HOTKEY_CODE_ID);
    #endif
}

size_t ConfigWidget::qtKeyToWin(size_t key) {
    // TODO: other maping or full keys list
    switch (key) {
    case Qt::Key_F9:
        return VK_F9;
        break;
    case Qt::Key_F10:
        return VK_F10;
        break;
    case Qt::Key_F11:
        return VK_F11;
        break;
    }
    qDebug() << "Hotkey not found in mapping!";
    return 0;
}

void ConfigWidget::registerHotkeyWin(const QString& str, size_t hotkeyId) {
    qDebug() << Q_FUNC_INFO;
    QKeySequence keys;
    keys = QKeySequence::fromString(str);
    size_t winMod = 0;
    size_t key = 0x42;

    qDebug() << "Str:   " << str;
    qDebug() << "Count: " << keys.count();

    // TODO: correctly process modifiers
    for (size_t i = 0; i != (size_t)keys.count(); i++) {
        if (keys[i] == Qt::CTRL) {
            winMod = MOD_CONTROL;
        } else if (keys[i] == Qt::ALT) {
            winMod = MOD_ALT;
        } else {
            key = qtKeyToWin(keys[i]);
        }
    }

    qDebug() << key;

    if (!RegisterHotKey((HWND)winId(), hotkeyId, winMod, key)) {
        qDebug() << "Error activating hothey!";
    }
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

    _ui.checkBoxLangDialogShow->setChecked(showsourcedialog);
}

bool ConfigWidget::winEvent (MSG * message, long * result) {
    Q_UNUSED(result);
    if (message->message == WM_HOTKEY) {
        switch (message->wParam) {
        case HOTKEY_FULL_ID:
            ((Application*)qApp)->processScreenshotFull();
            break;
        case HOTKEY_CODE_ID:
            ((Application*)qApp)->processCodeShare();
            break;
        case HOTKEY_PART_ID:
            ((Application*)qApp)->processScreenshotPart();
            break;
        }
    }
    return false;
}

bool ConfigWidget::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType);
    #ifdef Q_OS_WIN
    winEvent((MSG*)message, result);
    #endif
    return false;
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
