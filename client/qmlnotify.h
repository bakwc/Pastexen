#pragma once

#include <QObject>

class QQuickWindow;

class QmlNotify : public QObject
{
    Q_OBJECT
public:
    QmlNotify();

public slots:
    void show(QString title = "", QString message = "", int duration = 2000);

private:
    void init();

    QQuickWindow* qmlObject;
};
