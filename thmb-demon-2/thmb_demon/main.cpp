#include <iostream>

#include "redis_task.h"
#include "thumb_generator.h"

using namespace std;

int main(int argc, char** argv) {

    TThumbGenerator generator("C:\\koding\\font.ttf", 64, 64);
    generator.GenerateThumbnail(FT_Text, "C:\\koding\\image.txt", "C:\\koding\\thumb.png");

    return 0;

    try {
        TRedisTaskManager taskManager("dev.pastexen.com", 6378, 0);
        optional<TTask> task = taskManager.GetTask();
        if (task.is_initialized()) {
            cout << task->FilePath << "\n";
            if (task->Type == FT_Image) {
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
