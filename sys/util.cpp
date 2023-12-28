#include "util.h"

namespace util {

    float strToFloat ( std::string str ) {
        try {
            return std::stof(str);
        } catch (...) {
            qPrint("ERROR using stof! Attempted string:",str);
        }
        return -1.0f;
    }


    int strToInt(std::string str){
        try{
            return std::stoi(str);
        } catch (...){
            qPrint("ERROR using stoi! Attempted string:",str);
        }
        return -1;
    }

    void printMemUse(rusage& usageRef) {
        if(getrusage(RUSAGE_SELF, &usageRef) == 0){
            qPrint("Memory Usage:",usageRef.ru_maxrss,"kb");
        }
    }


}
