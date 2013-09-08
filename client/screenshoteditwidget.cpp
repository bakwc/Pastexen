#include <QDesktopWidget>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>

#include "screenshoteditwidget.h"
#include "ui_screenshoteditwidget.h"

ScreenshotEditWidget::ScreenshotEditWidget(QPixmap* source) :
    QDialog(NULL),
    _selectedTool(ST_CustomDraw),
    _toolActive(false),
    _color(Qt::red),
    _source(source),
    _newPixmap(*source),
    ui(new Ui::ScreenshotEditWidget)
{
    ui->setupUi(this);
    this->setGeometry(0, 0,
                      source->width() + ui->colorDisplayWidget->x() * 2,
                      ui->screenshotDisplayWidget->y() + source->height() +
                        ui->colorDisplayWidget->height() + 10);

    ui->screenshotDisplayWidget->setGeometry(ui->screenshotDisplayWidget->x(),
                                        ui->screenshotDisplayWidget->y(),
                                        source->width(), source->height());

    this->move(QDesktopWidget().availableGeometry().center().x() - (this->width() / 2),
               QDesktopWidget().availableGeometry().center().y() - (this->height() / 2));
    ui->buttonBox->setGeometry(ui->buttonBox->x(),
                               ui->screenshotDisplayWidget->y() + ui->screenshotDisplayWidget->height() + 10,
                               source->width() - ui->buttonBox->x() * 2,
                               ui->buttonBox->height());

    this->setFixedSize(this->size());
}

ScreenshotEditWidget::~ScreenshotEditWidget()
{
    delete ui;
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

    if (_toolActive && _selectedTool == ST_Rectangle) {
        QPen pen;
        pen.setColor(_color);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(ui->screenshotDisplayWidget->x() + _startToolPosition.x(),
                         ui->screenshotDisplayWidget->y() + _startToolPosition.y(),
                         _lastToolPosition.x() - _startToolPosition.x(),
                         _lastToolPosition.y() - _startToolPosition.y());
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
        case ST_CustomDraw:
            QPainter painter(&_newPixmap);
            QPen pen;
            pen.setColor(_color);
            pen.setWidth(3);
            painter.setPen(pen);
            painter.drawLine(_lastToolPosition, currentPos);
            break;
        }

        _lastToolPosition = currentPos;
        update();
    }
}

void ScreenshotEditWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() != Qt::RightButton &&
            event->button() != Qt::MidButton &&
            _toolActive == true &&
            _selectedTool == ST_Rectangle)
    {
        QPainter painter(&_newPixmap);
        QPen pen;
        pen.setColor(_color);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRect(_startToolPosition.x(),
                         _startToolPosition.y(),
                         _lastToolPosition.x() - _startToolPosition.x(),
                         _lastToolPosition.y() - _startToolPosition.y());
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
    }
}

void ScreenshotEditWidget::on_colorSelectButton_clicked() {
    QColor result = QColorDialog().getColor(_color);
    if (result.isValid()) {
        _color = result;
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

void ScreenshotEditWidget::on_buttonBox_accepted() {
    *_source = _newPixmap;
}
