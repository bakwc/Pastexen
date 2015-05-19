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
            "background-color: rgb(255, 255, 255);"
        "}"
        "TrayWindow {"
            "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 5px;"
            "border-color: black;"
            "padding: 3px;"
        "}"
        "#Text {"
            "min-height: 13pt;"
            "font: 10pt;"
        "}";

TrayWindow::TrayWindow() :
    QFrame(nullptr,
           Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(&text);
    layout->addWidget(&bar);

    text.setObjectName("Text");
    bar.setObjectName("ProgressBar");
    #if defined(Q_OS_MAC)
    bar.setFixedSize(180, 10);
    setFixedSize(220, 80);
    #else
    bar.setFixedSize(130, 14);
    setFixedSize(160, 60);
    #endif

    QRect dsktpSize = qApp->screens().at(0)->availableGeometry();
    if (dsktpSize.top() == 0) {
        // toolbar is on bottom
        move(dsktpSize.width() - width(), dsktpSize.height() - height());
    } else {
        // toolbar is on top
        move(dsktpSize.width() - width(), dsktpSize.top());
    }

    setStyleSheet(STYLESHEET);
}

void TrayWindow::showMessage( QString text, ETrayMessageType type, int interval, bool showProgressBar)
{
    // todo: show different images for different error types
    switch(type) {
    case TMT_None: break;
    case TMT_Info: break;
    case TMT_Error: text = tr("Error: ") + text;
    case TMT_Success: break;
    }

    this->text.setText(text);
    this->bar.setVisible(showProgressBar);
    this->bar.setValue(0);
    startTimer(interval);
    show();
    #if defined(Q_OS_MAC)
    this->activateWindow();
    this->raise();
    #endif
    QCoreApplication::processEvents();
}


void TrayWindow::showUploadMessage(QString text)
{
    this->text.setText(text);
    this->bar.setVisible(true);

    const QTcpSocket& socket = Application::network().socket();

    if (socket.isOpen() && socket.bytesToWrite() > 0) {
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
            this->text.setText("Done!");
            this->startTimer(3000);
            disconnect(this);
        });
    }
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
