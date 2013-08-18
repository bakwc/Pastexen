#ifndef TRAYWINDOW_H
#define TRAYWINDOW_H

#include <QFrame>
#include <QLabel>
#include <QProgressBar>

class TrayWindow : public QFrame
{
    Q_OBJECT
public:
    explicit TrayWindow();

    void showTextMessage(QString title = "", QString text = "", int interval = 2000);
    void showUploadMessage(QString title = "", QString text = "", QPixmap img = QPixmap(0, 0));
    void showUploadedMessage(QString title = "", QString text = "", QPixmap img = QPixmap(0, 0));

protected:
    void timerEvent(QTimerEvent *);
    int startTimer(int interval);
    
signals:
    
public slots:

private:
    QWidget* createTitle();
    QWidget* createContent();
    QWidget* createBottom();

    QLabel title;
    QLabel text;
    QProgressBar bar;
    
};

#endif // TRAYWINDOW_H
