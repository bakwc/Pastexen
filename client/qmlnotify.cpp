#include "qmlnotify.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QtQuickWidgets/QQuickWidget>
#include <QtQuick/QQuickWindow>
#include <QSurfaceFormat>

#include <iostream>

QmlNotify::QmlNotify()
{
    init();
}

void QmlNotify::init()
{
    QQmlComponent *qmlComponent = new QQmlComponent(new QQmlEngine());
    qmlComponent->loadUrl(QUrl("qrc:/main.qml"));

    if (!qmlComponent->isReady()) {
        std::cout << Q_FUNC_INFO << " QmlComponent is not ready: " << qmlComponent->errorString().toStdString() << std::flush;
        return;
    }

    QQuickWindow *qWindow = (QQuickWindow *)(qmlComponent->create());

    QSurfaceFormat surfaceFormat = qWindow->requestedFormat();
    qWindow->setFormat(surfaceFormat);
    qWindow->show();
}
