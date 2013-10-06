#include "application.h"
#include <QTimer>
#include <QTranslator>
#include <memory>

int main(int argc, char *argv[])
{
    std::unique_ptr<Application> app(new Application(argc, argv));

    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString(":/pastexen_") + locale);
    app->installTranslator(&translator);

    if (!app->pxAppInit()) {
        return 0;
    }
    return app->exec();
}
