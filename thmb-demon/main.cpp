#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

#include "application.h"

int main(int argc, char *argv[])
{
    #ifdef Q_OS_LINUX
    int d = daemon(0, 0);
    Q_UNUSED(d);
    #endif

    TApplication a(argc, argv);

    return a.exec();
}
