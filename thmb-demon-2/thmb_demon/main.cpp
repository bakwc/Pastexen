#include <iostream>

#include "redis_task.h"

using namespace std;

int main(int argc, char** argv) {
    try {
        TRedisTaskManager taskManager("dev.pastexen.com", 6378, 0);
        optional<TTask> task = taskManager.GetTask();
        if (task.is_initialized()) {
            cout << task->FilePath << "\n";
            if (task->Type == TTask::Image) {
                cout << "image\n";
            } else {
                cout << "text\n";
            }
        } else {
            cout << "No task!\n";
        }
    } catch (const TTaskException& t) {
        cout << "Exception: " << t.what() << "\n";
    }

    return 0;
}
