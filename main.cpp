#include "main.h"

static int secondsCount = 0;
static bool closeThreads = false;

int main () {

    if(UI::initialize()) return 1;

    std::thread t_sec (tick_sec);

    //main loop
    while(UI::mainLoopCondition()){
        UI::mainLoop();

    }

    //clean up
    closeThreads = true;
    UI::cleanUp();

    t_sec.join();

    return 0;

}


void tick_sec() {
    if(closeThreads) return;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    secondsCount += 1;
    util::qPrint(secondsCount);
    tick_sec();
}
