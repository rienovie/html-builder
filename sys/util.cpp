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




    template <typename T>
    void qPrint(T output) {
        std::cout << output << "\n";
    }

    template <typename T>
    void qPrint(T output0, T output1){
        std::cout << output0 << " " << output1 << "\n";
    }

    template <typename C, typename T>
    void qPrint(C output0, T output1){
        std::cout << output0 << " " << output1 << "\n";
    }


    template void qPrint<int>(int);
    template void qPrint<float>(float);
    template void qPrint<char const*>(char const*);
    template void qPrint<char const*>(char const*, char const*);
    template void qPrint<std::string>(std::string);
    template void qPrint<char>(char);
    template void qPrint<std::string>(std::string, std::string);
    template void qPrint<char const*, int>(char const*, int);
    template void qPrint<int>(int, int);
    template void qPrint<char const*, std::string>(char const*, std::string);
}
