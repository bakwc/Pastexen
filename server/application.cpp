#include "application.h"
#include <QDebug>

Application::Application(int argc, char **argv) :
    QCoreApplication(argc, argv)
{
    _args = arguments();
    _args.removeFirst();
}

bool Application::parseArgs()
{
    if (_args.size() == 0) {
        usage();
        return true;
    }

    if (_args[0] == "-h" || _args[0] == "--help") {
        usage();
        return false;
    }

    for (int it = 0; it < _args.size(); ++it) {
        if (_args[it] == "-log") {
            if (++it <= _args.size()-1) {
                _logFile = _args[it];
            } else {
                usage();
                return false;
            }
            continue;
        }

        if (_config.size() == 0)
            _config = _args[it];
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
