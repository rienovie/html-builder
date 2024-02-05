#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/resource.h>
#include <filesystem>
#include <math.h>



namespace util {


    struct int2d {
        int x, y;

        int2d(int _x, int _y) : x(_x), y(_y) {};
        int2d() : x(0), y(0) {};
    };

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

    //if bReturnIndex = false will return 0 if not found
    //if bReturnIndex = true will return -1 if not found
    template <typename T>
    int searchVector(std::vector<T>& inputVector, T toFind, bool bReturnIndex = false) {
        int iSize = inputVector.size();
        for(int i = 0; i < iSize; i++) {
            if(inputVector[i] == toFind) {
                return (i + (!bReturnIndex));
                //need to return at least 1 if found at index 0 when bReturnIndex is false
            }
        }
        return (0 - (bReturnIndex));
    }

    int strToInt(std::string str);
    float strToFloat(std::string str);
    std::vector<std::string> splitStringOnChar(std::string& sToSplit,char cSplit);
    void printMemUse(rusage& usageRef);
    bool hasPathPermission(std::filesystem::path pPath, bool bPrintErrors = false);
    std::string vectorToSingleStr(std::vector<std::string>& sFullVec,bool bAddNewLines = true);
    int lerpInt(int iStart, int iEnd, float fLerp);

}
