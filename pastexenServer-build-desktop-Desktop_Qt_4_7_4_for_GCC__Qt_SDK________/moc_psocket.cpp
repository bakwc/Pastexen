/****************************************************************************
** Meta object code from reading C++ file 'psocket.h'
**
** Created: Fri Sep 28 20:11:28 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../server/psocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'psocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_pSocket[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      26,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_pSocket[] = {
    "pSocket\0\0onDataReceived()\0onDisconnected()\0"
};

const QMetaObject pSocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_pSocket,
      qt_meta_data_pSocket, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &pSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *pSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *pSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pSocket))
        return static_cast<void*>(const_cast< pSocket*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int pSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onDataReceived(); break;
        case 1: onDisconnected(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
