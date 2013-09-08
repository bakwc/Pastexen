#pragma once

#include <QDialog>

namespace Ui {
class ScreenshotEditWidget;
}

enum ESelectedTool {
    ST_CustomDraw,
    ST_Rectangle
};

class ScreenshotEditWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ScreenshotEditWidget(QPixmap* source);
    ~ScreenshotEditWidget();
private slots:
    void on_colorSelectButton_clicked();
    void on_clearButton_clicked();
    void on_customDrawButton_clicked();
    void on_rectangleDrawButton_clicked();
    void on_buttonBox_accepted();

private:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void selectTool(ESelectedTool tool);
private:
    ESelectedTool _selectedTool;
    bool _toolActive;
    QPoint _lastToolPosition;
    QPoint _startToolPosition;
    QColor _color;
    QPixmap* _source;
    QPixmap _newPixmap;
    Ui::ScreenshotEditWidget* ui;
};
