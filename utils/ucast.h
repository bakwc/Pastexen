#pragma once

#include <QString>
#include "uexception.h"

template<typename T>
T FromString(const QString& str);

class UFromStringFormat {
public:
    UFromStringFormat(const QString& str = QString())
        : Str(str)
    {
    }
    template <typename T>
    inline operator T() const {
        return FromString<T>(Str);
    }
private:
    QString Str;
};

inline UFromStringFormat FromString(const QString& str) {
    return UFromStringFormat(str);
}

template <typename T>
inline QString ToString(const T& t) {
    return QString::number(t);
}

inline QString ToString(const QByteArray& byteArray) {
    return QString::fromUtf8(byteArray);
}
