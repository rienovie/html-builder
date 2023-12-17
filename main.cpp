#include "main.h"

//TODO change variable naming convention
//int = iVariableName
//bool = bVariableName
//string = sVariableName
//char = cVariableName
//etc

//TODO move ImGui windows into their own file / header
//TODO remove darkMode and make color customization available
//TODO set screen size by config

static bool closeThreads = false;

int main () {

    config cfg;

    if(UI::initialize()) { return 1; }

    std::thread t_sec (tick_sec);

    UI::setThemeByName(cfg.get("theme"));

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
    //util::qPrint("tick");
    tick_sec();
}
