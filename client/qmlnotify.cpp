#include "qmlnotify.h"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include <QDebug>

QmlNotify::QmlNotify()
{
    init();
}

void QmlNotify::show(QString title, QString message, int duration)
{
    qmlObject->show();
}

void QmlNotify::init()
{
    QQmlEngine *qmlEngine = new QQmlEngine();
    qmlEngine->rootContext()->setContextProperty("qmlObject", this);
    QQmlComponent *qmlComponent = new QQmlComponent(qmlEngine);
    qmlComponent->loadUrl(QUrl("qrc:/main.qml"));

    if (!qmlComponent->isReady()) {
        qWarning() << Q_FUNC_INFO << " QmlComponent is not ready: " << qmlComponent->errorString();
        return;
    }

    qmlObject = qobject_cast<QQuickWindow*>(qmlComponent->create());
}
