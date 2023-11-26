#include "main.h"

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
    util::qPrint("tick");
    tick_sec();
}
