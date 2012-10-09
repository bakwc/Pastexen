#include "application.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    app.initialize();
    return app.exec();
}
