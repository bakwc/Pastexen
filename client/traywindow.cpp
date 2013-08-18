#include "traywindow.h"
#include "QDesktopWidget"
#include "QLabel"
#include "application.h"
#include "QScreen"
#include "QGraphicsOpacityEffect"
#include "QLayout"
#include "QProgressBar"

const QString STYLESHEET =
        "* {"
            "background-color: rgb(100, 100, 100);"
        "}"
        "TrayWindow {"
            "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 5px;"
            "border-color: black;"
            "padding: 3px;"
        "}"
        "QLabel {"
            "color: white;"
        "}"
        "#TitleWidget {"
            "min-height: 15pt;"
        "}"
        "#TitleLabel {"
            "min-height: 15pt;"

            "font: 12pt;"
        "}";

TrayWindow::TrayWindow() :
    QFrame(nullptr,
           Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(createTitle());
    layout->addWidget(createContent());
    layout->addWidget(createBottom());

    title.setObjectName("TitleLabel");
    text.setObjectName("TextLabel");
    bar.setObjectName("ProgressBar");

    int w = 200, h = 140;
    int xi = 90, yi = 80;

    setFixedSize(w, h);

    QSize dsktpSize = qApp->screens().at(0)->size();
    move(dsktpSize.width() - width() - xi,
         dsktpSize.height() - height() - yi);

//    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(STYLESHEET);

    //    this->startTimer(5000);
}

void TrayWindow::showTextMessage(QString title, QString text, int interval)
{
    this->title.setText(title);
    this->text.setText(text);
    this->bar.setVisible(false);
    startTimer(interval);
    show();
}


void TrayWindow::showUploadMessage(QString title, QString text, QPixmap img)
{
    this->title.setText(title);
    this->text.setText(text);
    this->bar.setVisible(true);

    const QTcpSocket& socket = Application::network().socket();
    if (socket.isOpen() && socket.bytesToWrite() > 0) {
        qDebug() << Q_FUNC_INFO << "socket open";
        connect(&socket, &QTcpSocket::bytesWritten, [&](qint64 value) {
            int barValue;
            qint64 s = socket.bytesToWrite();
            if (s > 0) {
                double d = value / s;
                barValue = d*100;
            } else {
                barValue = 100;
            }

            bar.setValue(barValue);
        });
        connect(&Application::network(), &Network::linkReceived, [&]() {
            bar.setValue(100);
            this->text.setText("Successfull!");
            this->startTimer(2000);
            disconnect(this);
        });
    }

    startTimer(10000);
    show();
}


void TrayWindow::showUploadedMessage(QString title, QString text, QPixmap img)
{
    this->title.setText(title);
    this->text.setText(text);
    this->bar.setVisible(true);
    this->bar.setValue(100);

    this->startTimer(1000);
    show();
}


void TrayWindow::timerEvent(QTimerEvent *)
{
    hide();
}


int TrayWindow::startTimer(int interval)
{
    static int timerId = -1;
    if (timerId != -1) {
        killTimer(timerId);
    }
    timerId = QObject::startTimer(interval);
    return timerId;
}


QWidget* TrayWindow::createTitle() {
    QWidget *wdgt = new QWidget;
    wdgt->setObjectName("TitleWidget");
    QHBoxLayout* layout = new QHBoxLayout(wdgt);

    layout->addSpacing(20);
    layout->addWidget(&title);
//    layout->addWidget(closeBtn, 1, Qt::AlignTop);

    return wdgt;
}

QWidget* TrayWindow::createContent() {
    QWidget *wdgt = new QWidget;
    wdgt->setObjectName("ContentWidget");
    QHBoxLayout* layout = new QHBoxLayout(wdgt);

    layout->addWidget(&text);

    return wdgt;
}

QWidget* TrayWindow::createBottom() {
//    const QTcpSocket& socket = Application::network().socket();
//    if (socket.isOpen() && socket.size() > 0) {
//        qDebug() << Q_FUNC_INFO << "socket open";
//        connect(&socket, &QTcpSocket::bytesWritten, [&](qint64 value) {
//            qint64 s = socket.size();
//            double d = value / s;
//            bar->setValue(d*100);
//        });
//        connect(&Application::network(), &Network::linkReceived, [&]() {
//            bar->setValue(100);
//            this->startTimer(1000);
//        });
//    }

    return &bar;
}
