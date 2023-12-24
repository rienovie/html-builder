#include "main.h"

/*Variable Naming = prefixVariableName
 * bTestVariable = true;
 * i = int
 * b = bool
 * s = string
 * c = char
 * m = map
 * v = vector
 * f = float
 *
*/

//TODO move ImGui windows into their own file / header
//TODO remove darkMode and make color customization available

struct rusage memUsage;
static bool bCloseThreads = false;

int main () {

    config cfg;

    if(UI::initialize()) { return 1; }

    std::thread t_sec (tick_sec);
    std::thread t_long (tick_long);

    //UI::setThemeByName(cfg.get(config::system,"theme"));

    //main loop
    while(UI::mainLoopCondition()){
        UI::mainLoop();

    }

    //clean up
    bCloseThreads = true;
    UI::cleanUp();

    t_sec.detach();
    t_long.detach();

    return 0;

}


void tick_sec() {
    if( bCloseThreads ) return;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //util::qPrint("tick");
    UI::tick_sec();

    //recursive call
    tick_sec();
}

void tick_long() {
    if(bCloseThreads) return;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    util::printMemUse(memUsage);

    //recursive call
    tick_long();

}

