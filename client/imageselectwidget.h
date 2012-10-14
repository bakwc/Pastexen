#pragma once

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
