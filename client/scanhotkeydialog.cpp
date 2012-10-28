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
        QKeySequence seq;

        if (e->key() >= Qt::Key_Shift && e->key() <= Qt::Key_NumLock ||
                e->key() == -1) {
            seq = QKeySequence(e->modifiers());
        } else {
            seq = QKeySequence(e->key(), e->modifiers());
        }

        _key = rigthStringKey(seq.toString());

        _ui.lineEdit->setText(_key);
    }

    return QDialog::event(ev);
}

QString ScanHotkeyDialog::rigthStringKey(const QString &key)
{
    int index = key.indexOf(',');
    if (index == -1)
        return key;

    QString rightKey(key.mid(index + 2));
    rightKey.append(key.left(index));
    return rightKey;
}
