#include <QCoreApplication>
#include <QSettings>
#include <QTime>
#include "pserver.h"
#include "pthreadpool.h"
#include "psetting.h"
#include "psaver.h"
#include <cstdlib>
#include <ctime>
#include "logger.h"
#include <QFile>

void usage() {
    qDebug() << "Usage:\n"
             << "pastexenServer [config.file] [-log log.file]\n";
}

QStringList parseArguments(const QStringList& argsList, int pos = 0);

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QString pathToConfig;
    QFile logFile;

    QStringList args = app.arguments();
    args.removeFirst();

    qDebug() << "args:" << args;
//    switch(args.size()) {
//    case 1: {
//        usage();
//        break;
//    }
//    default: {
//        for (int i = 1; i < args.size(); ++i) {
//            const QString& arg = args[i];
//            if (arg[0] == '-') {
//                if (arg == "-h" || arg == "--help")
//                    usage();
//                else if (arg == "-log") {
//                    logFile.setFileName();
//                }
//                else {
//                    qDebug() << "Unknown argument:" << argv[1];
//                    usage();
//                }
//                return 0;
//            }
//        }
//    }
//    }

    Logger::configure(&logFile);

    Settings    setting(pathToConfig);
    pSaver      saver;
    pThreadPool pool(Settings::threads());

    pServer     server;

    app.exec();
}


//QStringList parseArguments(const QStringList &argsList, int pos)
//{


//}
