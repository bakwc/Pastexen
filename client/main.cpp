#include "application.h"

#if defined(Q_OS_WIN)
    #define _WIN32_WINNT 0x0500
    #include <windows.h>
    HWND WINAPI GetConsoleWindow(void);     // For hiding console in windows
#endif

int main(int argc, char *argv[])
{
    /*
    #if defined(Q_OS_WIN)
        HWND hwnd = GetConsoleWindow();
        ShowWindow(hwnd, 0);               // For hiding console in windows
    #endif
    */
    Application app(argc, argv);
    app.init();
    return app.exec();
}
