#include <QDesktopWidget>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>

#include "application.h"
#include "screenshoteditwidget.h"
#include "ui_screenshoteditwidget.h"

#include <qmath.h>

ScreenshotEditWidget::ScreenshotEditWidget(QPixmap* source) :
    QDialog(NULL),
    _selectedTool(ST_CustomDraw),
    _toolActive(false),
    _source(source),
    _newPixmap(*source),
    ui(new Ui::ScreenshotEditWidget)
{
    QString new_color = Application::settings().GetParameter("color", DEFAULT_COLOR);
    _color = QColor(new_color);
    ui->setupUi(this);
    this->setGeometry(0, 0,
                      source->width() + ui->colorDisplayWidget->x() * 2,
                      ui->screenshotDisplayWidget->y() + source->height() +
                        ui->colorDisplayWidget->height() + 10);

    int minWidth = ui->clearButton->x() + ui->clearButton->width() + ui->colorDisplayWidget->x();

    if (this->width() < minWidth) {
        this->setGeometry(this->x(), this->y(), minWidth, this->height());
    }

    ui->screenshotDisplayWidget->setGeometry(width() / 2 - source->width() / 2,
                                        ui->screenshotDisplayWidget->y(),
                                        source->width(), source->height());

    this->move(QDesktopWidget().availableGeometry().center().x() - (this->width() / 2),
               QDesktopWidget().availableGeometry().center().y() - (this->height() / 2));
    ui->buttonBox->setGeometry(ui->buttonBox->x(),
                               ui->screenshotDisplayWidget->y() + ui->screenshotDisplayWidget->height() + 10,
                               this->width() - ui->buttonBox->x() * 2 - 20,
                               ui->buttonBox->height());

    this->setFixedSize(this->size());
    this->show();
}

ScreenshotEditWidget::~ScreenshotEditWidget()
{
    delete ui;
}

static void DrawArrow(QPainter& painter, int x1, int y1, int x2, int y2) {
    if (x1 == x2 && y1 == y2) {
        return;
    }
    float abs = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    float normX = float(x2 - x1) * 15 / abs;
    float normY = float(y2 - y1) * 15 / abs;
    int intersectionX = x2 - normX;
    int intersectionY = y2 - normY;
    float l1x = -normY + intersectionX;
    float l1y = normX + intersectionY;
    float l2x = normY + intersectionX;
    float l2y = -normX + intersectionY;

    painter.drawLine(x1, y1, x2, y2);
    painter.drawLine(x2, y2, l1x, l1y);
    painter.drawLine(x2, y2, l2x, l2y);
}

void ScreenshotEditWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(ui->screenshotDisplayWidget->x(), ui->screenshotDisplayWidget->y(),
                       _source->width(), _source->height(), _newPixmap);
    painter.fillRect(ui->colorDisplayWidget->x(), ui->colorDisplayWidget->y(),
                     ui->colorDisplayWidget->width(), ui->colorDisplayWidget->height(), _color);
    painter.drawRect(ui->screenshotDisplayWidget->x(), ui->screenshotDisplayWidget->y(),
                     ui->screenshotDisplayWidget->width(), ui->screenshotDisplayWidget->height());

    if (_toolActive) {
        QPen pen;
        pen.setColor(_color);
        pen.setWidth(3);
        painter.setPen(pen);
        if (_selectedTool == ST_Rectangle) {
            painter.drawRect(ui->screenshotDisplayWidget->x() + _startToolPosition.x(),
                             ui->screenshotDisplayWidget->y() + _startToolPosition.y(),
                             _lastToolPosition.x() - _startToolPosition.x(),
                             _lastToolPosition.y() - _startToolPosition.y());
        } else if (_selectedTool == ST_Arrow) {
            DrawArrow(painter,
                        ui->screenshotDisplayWidget->x() + _startToolPosition.x(),
                        ui->screenshotDisplayWidget->y() + _startToolPosition.y(),
                        ui->screenshotDisplayWidget->x() + _lastToolPosition.x(),
                        ui->screenshotDisplayWidget->y() + _lastToolPosition.y());
        }
    }
}

void ScreenshotEditWidget::mousePressEvent(QMouseEvent* event) {
    _toolActive = true;
    _lastToolPosition = QPoint(event->x() - ui->screenshotDisplayWidget->x(),
                               event->y() - ui->screenshotDisplayWidget->y());
    _startToolPosition = _lastToolPosition;
}

void ScreenshotEditWidget::mouseMoveEvent(QMouseEvent* event) {
    if (_toolActive) {
        QPoint currentPos(event->x() - ui->screenshotDisplayWidget->x(),
                          event->y() - ui->screenshotDisplayWidget->y());

        switch (_selectedTool) {
        case ST_CustomDraw: {
            QPainter painter(&_newPixmap);
            QPen pen;
            pen.setColor(_color);
            pen.setWidth(3);
            painter.setPen(pen);
            painter.drawLine(_lastToolPosition, currentPos);
            break;
        }
        default:
            // to suppress warnings
            break;
        }

        _lastToolPosition = currentPos;
        update();
    }
}

void ScreenshotEditWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() != Qt::RightButton &&
            event->button() != Qt::MidButton &&
            _toolActive == true)
    {
        QPainter painter(&_newPixmap);
        QPen pen;
        pen.setColor(_color);
        pen.setWidth(3);
        painter.setPen(pen);
        if (_selectedTool == ST_Rectangle) {
            painter.drawRect(_startToolPosition.x(),
                             _startToolPosition.y(),
                             _lastToolPosition.x() - _startToolPosition.x(),
                             _lastToolPosition.y() - _startToolPosition.y());
        } else if (_selectedTool == ST_Arrow) {
            DrawArrow(painter,
                        _startToolPosition.x(),
                        _startToolPosition.y(),
                        _lastToolPosition.x(),
                        _lastToolPosition.y());
        }
    }
    _toolActive = false;
    update();
}

void ScreenshotEditWidget::selectTool(ESelectedTool tool) {
    if (tool != _selectedTool) {
        // Uncheck previous tool
        switch (_selectedTool) {
        case ST_CustomDraw:
            ui->customDrawButton->setChecked(false);
            break;
        case ST_Rectangle:
            ui->rectangleDrawButton->setChecked(false);
            break;
        case ST_Arrow:
            ui->arrowDrawButton->setChecked(false);
            break;
        }
        _selectedTool = tool;
    }

    // Check new tool
    switch(_selectedTool) {
    case ST_CustomDraw:
        ui->customDrawButton->setChecked(true);
        break;
    case ST_Rectangle:
        ui->rectangleDrawButton->setChecked(true);
        break;
    case ST_Arrow:
        ui->arrowDrawButton->setChecked(true);
        break;
    }
}

void ScreenshotEditWidget::on_colorSelectButton_clicked() {
    QColor result = QColorDialog().getColor(_color);
    if (result.isValid()) {
        _color = result;
        Application::settings().SetParameter("color", _color.name());
        Application::settings().Save();
    }
}

void ScreenshotEditWidget::on_clearButton_clicked() {
    _newPixmap = *_source;
    update();
}

void ScreenshotEditWidget::on_customDrawButton_clicked() {
    selectTool(ST_CustomDraw);
}

void ScreenshotEditWidget::on_rectangleDrawButton_clicked() {
    selectTool(ST_Rectangle);
}

void ScreenshotEditWidget::on_arrowDrawButton_clicked() {
    selectTool(ST_Arrow);
}

void ScreenshotEditWidget::on_buttonBox_accepted() {
    *_source = _newPixmap;
}
