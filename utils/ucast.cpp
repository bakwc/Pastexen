#ifndef EPSILON_SERVER
#include <QtGui/QColor>
#endif

#include "ucast.h"

template< >
quint8 FromString<quint8>(const QString& str){
    bool ok = false;
    int res = str.toUInt(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
int FromString<int>(const QString& str){
    bool ok = false;
    int res = str.toInt(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
double FromString<double>(const QString& str){
    bool ok = false;
    double res = str.toDouble(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
float FromString<float>(const QString& str){
    bool ok = false;
    float res = str.toFloat(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
uint FromString<uint>(const QString& str){
    bool ok = false;
    uint res = str.toUInt(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
ushort FromString<unsigned short>(const QString& str){
    bool ok = false;
    ushort res = str.toUShort(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
ulong FromString<unsigned long>(const QString& str){
    bool ok = false;
    ulong res = str.toULong(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
unsigned long long FromString<unsigned long long>(const QString& str){
    bool ok = false;
    unsigned long long res = str.toULongLong(&ok);
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

template< >
bool FromString<bool>(const QString& str){
    bool res = false;
    if (str == "1" || str == "true") {
       res = true;
    } else if (str == "0" || str == "false") {
       res = false;
    } else {
       throw UException("Cast error");
    }
    return res;
}

template< >
QString FromString<QString>(const QString& str){
    return str;
}

template< >
QByteArray FromString<QByteArray>(const QString& str){
    bool ok = false;
    QByteArray res = str.toUtf8();
    if (!ok) {
       throw UException("Cast error");
    }
    return res;
}

#ifndef EPSILON_SERVER
template< >
QColor FromString<QColor>(const QString& str){
    return QColor(str);
}
#endif
