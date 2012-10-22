#include "application.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    Application *app = new Application(argc, argv);
    if (!app->pxAppInit()) {
        return 0;
    }
    return app->exec();
}
