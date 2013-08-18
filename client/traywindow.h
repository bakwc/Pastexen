#pragma once

#include <QFrame>
#include <QLabel>
#include <QProgressBar>

enum ETrayMessageType {
    TMT_None,
    TMT_Info,
    TMT_Success,
    TMT_Error
};

class TrayWindow : public QFrame
{
    Q_OBJECT
public:
    explicit TrayWindow();
    void showMessage(QString text, ETrayMessageType type = TMT_None,
                     int interval = 3000,
                     bool showProgressBar = false);
    void showUploadMessage(QString text);
protected:
    void timerEvent(QTimerEvent *);
    int startTimer(int interval);
private:
    QLabel text;
    QProgressBar bar;
};
