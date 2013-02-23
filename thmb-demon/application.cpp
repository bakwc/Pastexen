#include <QString>
#include <QStringList>
#include <QPainter>
#include <QFile>
#include <QPixmap>
#include <QTextStream>

#include "application.h"

TApplication::TApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    qDebug() << Q_FUNC_INFO;
    this->startTimer(1000);
    ThumbDirectory = "/data/thumbs/";
}

void TApplication::timerEvent(QTimerEvent *) {
    QVector<size_t> tasks;
    if (!Redis.Connect("127.0.0.1", 6379)) {
        qDebug() << "redis server unavailable";
    } else {
        QStringList record = Redis.Query("SPOP thmb_tasks");
        if (record.size() != 2 || record[1] == "NULL") {
            Redis.Disconnect();
            return;
        }
        QString fileId = record[1];

        record = Redis.Query("HGET file_" + fileId + " path");
        if (record.size() != 2 || record[1] == "NULL") {
            Redis.Disconnect();
            return;
        }

        QString fileName = record[1];
        Redis.Disconnect();

        GenerateThmb(fileName);
    }

}

void TApplication::GenerateThmb(const QString &fileName) {
    QImage img;
    if (fileName.endsWith(".png") ||
            fileName.endsWith(".jpg"))
    {
        img = GenerateImgThmb(fileName);
    } else {
        img = GenerateTxtThmb(fileName);
    }
    if (img.width() == 64 && img.height() == 64) {
        QString thmbFileName = ThumbDirectory +
               fileName.mid(fileName.lastIndexOf("/") + 1) +
                + ".png";
        img.save(thmbFileName);
    }
}

QImage TApplication::GenerateImgThmb(const QString& fileName) {
    QImage thmb(fileName);
    if (thmb.isNull()) {
        qDebug() << "Error loading " << fileName;
        return QImage();
    }
    thmb = thmb.scaled(64, 64, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation).copy(0, 0, 64, 64);
    return thmb;
}

QImage TApplication::GenerateTxtThmb(const QString& fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << fileName;
    }

    QTextStream in(&file);
    QStringList text;
    while(!in.atEnd()) {
        QString line = in.readLine();
        text.append(line);
    }

    QPixmap thmb(64, 64);
    thmb.fill(Qt::black);
    QPainter painter(&thmb);
    painter.setPen(Qt::white);
    QFont font;
    font.setPointSize(6);
    painter.setFont(font);
    int i = -1;
    int count = 0;
    while (i < text.size() - 1 && count <= 8) {
       i++;
       QString str = text[i].trimmed();
       if (str.length() == 0) {
           continue;
       }
       count++;
       painter.drawText(8, 9 + count * 8, str);
    }

    return thmb.toImage();
}

