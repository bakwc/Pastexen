#pragma once

#include <QWidget>
#include <QAbstractNativeEventFilter>
#include <QSet>

#include "ukeysequence.h"
#include "uexception.h"

class UGlobalHotkeys : public QWidget
{
    Q_OBJECT
public:
    explicit UGlobalHotkeys(QWidget *parent = 0);
    void RegisterHotkey(const QString& keySeq, size_t id = 1);
    void RegisterHotkey(const UKeySequence& keySeq, size_t id = 1);
    void UnregisterHotkey(size_t id = 1);
    ~UGlobalHotkeys();
protected:
    #if defined(Q_OS_WIN)
    bool winEvent (MSG * message, long * result);
    #elif defined(Q_OS_LINUX)
    #endif
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
signals:
    void Activated(size_t id);
private:
    QSet<size_t> Registered;
};
