#include "main.h"

/* Variable / Function Naming = prefixVariableName
 * bTestVariable = true;
 * i = int
 * b = bool
 * s = string
 * c = char
 * m = map
 * v = vector
 * f = float
 * p = path (directory / filesystem path)
 * w = window includes Begin() and End()
 * sw = subWindow assumes it's within a Begin() and End()
 *
*/

struct rusage memUsage;
static bool bCloseThreads = false;

int main () {

    auto classCFG = new config;
    UI* classUI = NULL;
    auto classHTML = new html;
    auto classFetch = new fetch;

    try {
        classUI = new UI;
    } catch (std::string e) {
        util::qPrint(e);
        return 1;
    } catch (...) {
        util::qPrint("Unknown Throw in UI initialization");
        return 1;
    }

    std::thread t_sec (tick_sec);
    std::thread t_long (tick_long);

    //main loop
    while(UI::mainLoopCondition()){
        UI::mainLoop();
    }

    //clean up
    bCloseThreads = true;
    delete classCFG;
    delete classUI;
    delete classHTML;
    delete classFetch;
    t_sec.detach();
    t_long.detach();

    util::qPrint("Process End");

    return 0;

}


void tick_sec() {
    if( bCloseThreads ) return;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if( bCloseThreads ) return;
    //util::qPrint("tick");

    UI::tick_sec();
    config::checkIfShouldSaveConfigs();
    tick_sec();
}

void tick_long() {
    if( bCloseThreads ) return;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if( bCloseThreads ) return;
    //util::printMemUse(memUsage);
    //config::checkIfShouldSaveConfigs();

    tick_long();

}

