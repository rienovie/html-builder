#include "util.h"

namespace util {

    int strToInt(std::string str){
        try{
            return std::stoi(str);
        } catch (...){
            std::string eStr = "ERROR using stoi! Attempted string:";
            qPrint(eStr,str);
        }
        return -1;
    }

}
