#ifndef IMAGESELECTWIDGET_H
#define IMAGESELECTWIDGET_H

#include <QDialog>
#include <QPoint>

class ImageSelectWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ImageSelectWidget(QPixmap *source);
private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QPixmap *_source;
    bool _isSelecting;
    QPoint _startPoint;
    QPoint _endPoint;
};

#endif // IMAGESELECTWIDGET_H
