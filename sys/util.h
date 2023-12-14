#pragma once

#include <iostream>
#include <string>

namespace util {
    int strToInt(std::string str);

    //quick print
    template <typename T>
    void qPrint(T output) {
        std::cout << output << "\n";
    }
    template <typename T, typename... Args>
    void qPrint(T output, Args... args){
        std::cout << output << " ";
        qPrint(args...);
    }

}
