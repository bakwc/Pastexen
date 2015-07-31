#include "application.h"
#include <QTimer>
#include <QTranslator>
#include <QDebug>
#include <QtDebug>
#include <memory>
#include <iostream>

#include <QtGlobal>

int main(int argc, char *argv[])
{
    std::unique_ptr<Application> app(new Application(argc, argv));

    QString locale = QLocale::system().name();

    QTranslator translator;
    if (locale == "ru" || locale == "ru_RU") {
        translator.load(":/pastexen_ru");
        app->installTranslator(&translator);
    }

    if (!app->pxAppInit()) {
        return 0;
    }

    qDebug() << "Pastexen is running";

    return app->exec();
}
