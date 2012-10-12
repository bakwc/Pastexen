#include "application.h"
#include <QDebug>

Application::Application(int argc, char **argv) :
    QCoreApplication(argc, argv)
{
    /*qDebug() << "args" << */arguments();
}

bool Application::parseArgs()
{
    QStringList args = arguments();
    args.removeFirst();

    if (args.size() == 0) {
        usage();
        return true;
    }

    if (args[0] == "-h" || args[0] == "--help") {
        usage();
        return false;
    }

    for (int it = 0; it < args.size(); ++it) {
        if (args[it] == "-log") {
            if (++it <= args.size()-1) {
                _logFile = args[it];
            } else {
                usage();
                return false;
            }
            continue;
        }

        if (_config.size() == 0)
            _config = args[it];
        else {
            usage();
            return false;
        }
    }

    return true;
}

void Application::usage()
{
    qDebug() << "Usage:\n"
             << "pastexenServer [config.file] [-log log.file]\n";
}
