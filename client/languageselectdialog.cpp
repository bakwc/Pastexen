#include "languageselectdialog.h"
#include "ui_languageselectdialog.h"
#include "defines.h"

LanguageSelectDialog::LanguageSelectDialog(QSettings *settings, QWidget *parent)
    : QDialog(parent)
    , _settings(settings)
    , ui(new Ui::LanguageSelectDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    showTypes();
    auto sourcestype = _settings->value("general/sourcetype", DEFAULT_SOURCES_TYPE).toString();
    int srcIndex = ui->comboLanguageType->findData(sourcestype);
    if (srcIndex != -1) {
        ui->comboLanguageType->setCurrentIndex(srcIndex);
    }
}

LanguageSelectDialog::~LanguageSelectDialog()
{
    delete ui;
}

void LanguageSelectDialog::showTypes()
{
    ui->comboLanguageType->addItem(tr("Plain text"), QString("txt"));
    ui->comboLanguageType->addItem(tr("C++"), QString("cpp"));
    ui->comboLanguageType->addItem(tr("Pascal"), QString("pas"));
}

void LanguageSelectDialog::on_buttonBox_accepted()
{
    _settings->setValue("general/sourcetype" ,ui->comboLanguageType->itemData(ui->comboLanguageType->currentIndex()).toString());
}
