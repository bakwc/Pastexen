#pragma once

#include <QSize>
#include <QList>
//------------------------------------------------------------------------------
class QWidget;
//------------------------------------------------------------------------------
namespace utils {
//------------------------------------------------------------------------------
class DisplayMode {
public:
    DisplayMode(int width, int height)
        : m_size(QSize(width, height))
    {}

    DisplayMode(const DisplayMode& mode)
        : m_size(mode.m_size)
    {}

    inline QSize size() const {
        return m_size;
    }
    inline int width() const {
        return m_size.width();
    }
    inline int height() const {
        return m_size.height();
    }

private:
    QSize m_size;
};
//------------------------------------------------------------------------------
class UFullscreenWrapper {
public:
    typedef QList<DisplayMode> DisplayModes;
public:
    explicit UFullscreenWrapper(QWidget* parent);

    DisplayModes enumModes();
    bool changeToMode(const DisplayMode& mode);
    bool changeToMode(int width, int height);
    bool restoreMode();

#if defined(Q_WS_X11) && defined(USE_XRANDR)
private:
    int findModeId(int width, int height);
#endif

private:
    DisplayModes m_displayModes;
    QWidget* m_parent;
#if defined(Q_WS_X11) && defined(USE_XRANDR)
    bool m_extensionFound;
#endif
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
