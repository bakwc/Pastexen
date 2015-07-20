#pragma once

#include <QString>
#include <QtGlobal>

const QString SETTINGS_FILE =
        #ifdef Q_OS_WS_WIN
        "pastexen.ini";
        #else
        ".pastexen.conf";
        #endif
const QString DEFAULT_HOTKEY_FULL = "F11";
const QString DEFAULT_HOTKEY_PART = "F10";
const QString DEFAULT_HOTKEY_CODE = "F9";
const QString DEFAULT_IMAGE_TYPE = "png";
const QString DEFAULT_SOURCES_TYPE = "txt";
const QString DEFAULT_COLOR = "red";
const QString APP_RELEASE = "2";
const QString APP_NAME = "Pastexen v" + APP_RELEASE;
const QString DEFAULT_MODE = "release";
const QString MAC_APP_ID = "com.pastexen";
const bool    DEFAULT_SHOW_SOURCES_CONF_DIALOG = true;
const bool    DEFAULT_SHOW_EDIT_SCREENSHOT = false;
const bool    DEFAULT_AUTOSTART = true;
const bool    DEFAULT_SCALE_SCREENSHOT = true;

const size_t HOTKEY_FULL_ID = 100;
const size_t HOTKEY_PART_ID = 101;
const size_t HOTKEY_CODE_ID = 102;
