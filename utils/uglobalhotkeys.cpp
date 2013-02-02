#include <QtCore>
#if defined(Q_OS_WIN)
#include <windows.h>
#include "winhotkeymap.h"
#endif
#include "uglobalhotkeys.h"


UGlobalHotkeys::UGlobalHotkeys(QWidget *parent)
    : QWidget(parent)
{
}

void UGlobalHotkeys::RegisterHotkey(const QString& keySeq, size_t id) {
    RegisterHotkey(UKeySequence(keySeq), id);
}

void UGlobalHotkeys::RegisterHotkey(const UKeySequence& keySeq, size_t id) {
    if (keySeq.Size() == 0) {
        throw UException("Empty hotkeys");
    }
    if (Registered.find(id) != Registered.end()) {
        UnregisterHotkey(id);
    }
    #if defined(Q_OS_WIN)
    size_t winMod = 0;
    size_t key = VK_F2;

    for (size_t i = 0; i != keySeq.Size(); i++) {
        if (keySeq[i] == Qt::Key_Control) {
            winMod |= MOD_CONTROL;
        } else if (keySeq[i] == Qt::Key_Alt) {
            winMod |= MOD_ALT;
        } else if (keySeq[i] == Qt::Key_Shift) {
            winMod |= MOD_SHIFT;
        } else {
            key = QtKeyToWin(keySeq[i]);
        }
    }

    if (!RegisterHotKey((HWND)winId(), id, winMod, key)) {
        qDebug() << "Error activating hotkey!";
    } else {
        Registered.insert(id);
    }
    #endif
}

void UGlobalHotkeys::UnregisterHotkey(size_t id) {
    Q_ASSERT(Registered.find(id) != Registered.end() && "Unregistered hotkey");
    #if defined(Q_OS_WIN)
    UnregisterHotKey((HWND)winId(), id);
    #endif
    Registered.remove(id);
}

UGlobalHotkeys::~UGlobalHotkeys() {
    for (QSet<size_t>::iterator i = Registered.begin(); i != Registered.end(); i++) {
        #if defined(Q_OS_WIN)
        UnregisterHotKey((HWND)winId(), *i);
        #endif
    }
}

#if defined(Q_OS_WIN)
bool UGlobalHotkeys::winEvent(MSG * message, long * result) {
    Q_UNUSED(result);
    if (message->message == WM_HOTKEY) {
        size_t id = message->wParam;
        Q_ASSERT(Registered.find(id) != Registered.end() && "Unregistered hotkey");
        emit Activated(id);
    }
    return false;
}

#elif defined(Q_OS_LINUX)
#endif
bool UGlobalHotkeys::nativeEvent(const QByteArray &eventType,
                                       void *message, long *result)
{
    Q_UNUSED(eventType);
    #ifdef Q_OS_WIN
    return winEvent((MSG*)message, result);
    #endif
}
