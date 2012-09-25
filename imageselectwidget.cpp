#include "imageselectwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QRect>

ImageSelectWidget::ImageSelectWidget(QPixmap *source):
    QDialog(0),
    _source(source)
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
    this->setGeometry(0,0,source->width(),source->height());
    this->setCursor(Qt::CrossCursor);
}

void ImageSelectWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );
    QPainter painter( this );
    painter.drawPixmap(0,0,_source->width(),_source->height(),*_source);
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

        *_source = _source->copy(x, y, w, h);
        this->accept();
    }
}
