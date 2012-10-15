#pragma once

#include <QCoreApplication>
#include <QStringList>

class Application : public QCoreApplication
{
    Q_OBJECT

    QString _config;
    QString _logFile;

    enum ARGTYPE {
        ARGTYPE_STR = 1,
        ARGTYPE_ARG = 2
    };

public:
    explicit Application(int argc, char** argv);

    QString config() { return _config; }
    QString logFile() { return _logFile; }
    bool parseArgs();
    
signals:
    
public slots:

private:
    QStringList _args;

    void usage();
};
