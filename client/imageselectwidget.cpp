#include "imageselectwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

ImageSelectWidget::ImageSelectWidget(QPixmap *source):
    QDialog(0),
    _source(source)
{
    QRect screenSize = QApplication::desktop()->screenGeometry();
    double ratio = qApp->devicePixelRatio();
    _scaledSource = _source->scaled(screenSize.width() * ratio, screenSize.height() * ratio);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    this->setGeometry(0,0,_scaledSource.width() / ratio, _scaledSource.height() / ratio);
    this->setCursor(Qt::CrossCursor);
    this->show();
    this->activateWindow();
    this->raise();
    startTimer(300);
}

void ImageSelectWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );
    QPainter painter( this );
    painter.drawPixmap(0,0, _scaledSource);
    if (_isSelecting)
    {
        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(_startPoint.x(), _startPoint.y(),
                         _endPoint.x()-_startPoint.x(), _endPoint.y()-_startPoint.y());
    }

}

void ImageSelectWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        _isSelecting = true;
        _startPoint = event->pos();
        _endPoint = _startPoint;
    }
}

void ImageSelectWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_isSelecting)
    {
        _endPoint = event->pos();
        this->update();
    }
}

void ImageSelectWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton || event->button()==Qt::MidButton)
        this->reject();
    else if (event->button()==Qt::LeftButton)
    {
        qint32 x, y, w, h, x1, y1, x2, y2;
        x1=_startPoint.x();
        x2=_endPoint.x();
        y1=_startPoint.y();
        y2=_endPoint.y();

        x = x1<x2 ? x1 : x2;
        y = y1<y2 ? y1 : y2;
        w = x1<x2 ? x2-x1 : x1-x2;
        h = y1<y2 ? y2-y1 : y1-y2;

        double ratio = qApp->devicePixelRatio();
        double ratioX = double(_source->width()) / double(_scaledSource.width());
        double ratioY = double(_source->height()) / double(_scaledSource.height());
        *_source = _source->copy(x * ratio * ratioX, y * ratio * ratioY, w * ratio * ratioX, h * ratio * ratioY);
        this->accept();
    }
}

void ImageSelectWidget::timerEvent(QTimerEvent *) {
    QRect screenSize = QApplication::desktop()->screenGeometry();
    double ratio = qApp->devicePixelRatio();

    if (screenSize.width() * ratio != _scaledSource.width() ||
        screenSize.height() * ratio != _scaledSource.height())
    {
        _scaledSource = _source->scaled(screenSize.width() * ratio, screenSize.height() * ratio,
                                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        this->setGeometry(0,0,_scaledSource.width() / ratio, _scaledSource.height() / ratio);
    }
}
