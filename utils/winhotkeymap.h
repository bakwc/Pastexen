#include <QtCore>

#if defined(Q_OS_WIN)
size_t QtKeyToWin(size_t key) {
    // TODO: other maping or full keys list

    if (key >= 0x01000030 && key <= 0x01000047) {
        return VK_F1 + (key - Qt::Key_F1);
    }

    qDebug() << "Hotkey not found in mapping!";
    return 0;
}

#endif
