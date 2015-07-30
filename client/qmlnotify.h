#ifndef QMLNOTIFY_H
#define QMLNOTIFY_H

#include <QObject>


class QmlNotify : public QObject
{
    Q_OBJECT
public:
    QmlNotify();


private:
    void init();
};

#endif // QMLNOTIFY_H
