#pragma once

#include <QDebug>
#include <memory>

class UDebugOut
{
public:
    UDebugOut() : Debug(new QDebug(QtDebugMsg)) {}

    template<typename T>
    UDebugOut& operator << (const T& obj) {
#ifndef QT_NO_DEBUG
        *Debug << obj;
#else
        Q_UNUSED(obj);
#endif
        return *this;
    }

private:
    std::unique_ptr<QDebug> Debug;
};

#define UDebug (UDebugOut())
