#pragma once

#include <QString>
#include <QtGlobal>

const QString SETTINGS_FILE =
        #ifdef Q_OS_WS_WIN
        "config.ini";
        #else
        ".pastexen.conf";
        #endif
const QString DEFAULT_HOTKEY_FULL = "F11";
const QString DEFAULT_HOTKEY_PART = "F10";
const QString DEFAULT_HOTKEY_CODE = "F9";
const QString DEFAULT_IMAGE_TYPE = "png";
const QString DEFAULT_SOURCES_TYPE = "txt";
const QString APP_NAME = "Pastexen 0.2";
const bool    DEFAULT_SHOW_SOURCES_CONF_DIALOG = false;
