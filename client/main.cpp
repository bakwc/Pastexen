#include "application.h"
#include <QTimer>
#include <QTranslator>

int main(int argc, char *argv[])
{
    Application *app = new Application(argc, argv);

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString(":/pastexen_") + locale);
    app->installTranslator(&translator);

    if (!app->pxAppInit()) {
        return 0;
    }

    return app->exec();
}
