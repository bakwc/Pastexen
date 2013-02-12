#pragma once

#include <QApplication>
#include <QVector>
#include <QImage>
#include <QPixmap>

#include "../server/redisclient.h"

class TApplication : public QApplication
{
    Q_OBJECT
public:
    TApplication(int argc, char *argv[]);
    void timerEvent(QTimerEvent *);
private:
    void GenerateThmb(const QString& fileName);
    QImage GenerateImgThmb(const QString& fileName);
    QImage GenerateTxtThmb(const QString& fileName);
private:
    QString ThumbDirectory;
    TRedisClient Redis;
};
