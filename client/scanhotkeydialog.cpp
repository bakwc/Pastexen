#include "scanhotkeydialog.h"
#include <QDebug>

ScanHotkeyDialog::ScanHotkeyDialog(QWidget *parent) :
    QDialog(parent)
{
    _ui.setupUi(this);
    _ui.lineEdit->setReadOnly(true);
    this->setFixedSize(this->size());
}

bool ScanHotkeyDialog::event(QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress) {
        QKeyEvent* e = dynamic_cast<QKeyEvent*>(ev);

        if (Keys.GetSimpleKeys().size() < 1) {
            Keys.AddKey(e->key());
        }
        Keys.AddModifiers(e->modifiers());

        _ui.lineEdit->setText(Keys.ToString());
    }

    return QDialog::event(ev);
}
