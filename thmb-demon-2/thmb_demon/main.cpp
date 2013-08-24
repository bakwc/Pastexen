#include <iostream>
#include <memory>

#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <utils/usettings.h>

#include "redis_task.h"
#include "thumb_generator.h"

using namespace std;

string GetFileName(const string& fullPath) {
    vector<string> parts;
    boost::algorithm::split(parts, fullPath, boost::algorithm::is_any_of("/\\"));
    if (parts.empty()) {
        return "";
    }
    return parts[parts.size() - 1];
}

class TThumbDemon {
public:
    TThumbDemon(const string& configFile);
    void Run();
private:
    USettings Settings;
    unique_ptr<TRedisTaskManager> TaskManager;
    unique_ptr<TThumbGenerator> ThumbGenerator;
    string ThumbDir;
};



TThumbDemon::TThumbDemon(const string& configFile) {
    Settings.Load(configFile);
    string redisHost = Settings.GetParameter("redis_host");
    uint16_t redisPort = Settings.GetParameter("redis_port");
    size_t redisDbNum = Settings.GetParameter("redis_db_num");
    TaskManager.reset(new TRedisTaskManager(redisHost, redisPort, redisDbNum));

    string fontFile = Settings.GetParameter("font_file");
    size_t thumbWidth = Settings.GetParameter("thumb_width");
    size_t thumbHeight = Settings.GetParameter("thumb_height");
    ThumbGenerator.reset(new TThumbGenerator(fontFile, thumbWidth, thumbHeight));

    string thumbDir = Settings.GetParameter("thumb_dir");

    ThumbDir = thumbDir;
}

void TThumbDemon::Run() {
    while (true) {
        try {
            optional<TTask> task = TaskManager->GetTask();
            if (task.is_initialized()) {
                string destFile = ThumbDir + "/" + GetFileName(task->FilePath);
                ThumbGenerator->GenerateThumbnail(task->Type, task->FilePath, destFile);
            }
        } catch(const std::exception& e) {
            cerr << "Error processing task: " << e.what() << "\n";
        }

        boost::this_thread::sleep_for(chrono::milliseconds(500));
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << " Usage: ./thumb_demon config\n\n";
        return 42;
    }
    try {
        TThumbDemon demon(argv[1]);
        demon.Run();
    } catch (const std::exception& e) {
        cerr << "Failed to start: " << e.what() << "\n";
        return 42;
    }
    return 0;
}

