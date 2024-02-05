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

    std::vector<std::string> splitStringOnChar ( std::string& sToSplit, char cSplit ) {
        std::vector<std::string> vOutput;
        std::string sBuild = "";
        int iStrLength = sToSplit.length();

        for(int i = 0; i < iStrLength; i++) {
            if(sToSplit[i] == cSplit) {
                if(sBuild.length() == 0) { continue; } //if is blank don't add
                vOutput.push_back(sBuild);
                sBuild.clear();
            } else { sBuild.push_back(sToSplit[i]); }
        }
        if(sBuild.length() != 0) { vOutput.push_back(sBuild); }

        return vOutput;
    }

    bool hasPathPermission ( std::filesystem::path pPath, bool bPrintErrors ) {
        try {
            if(std::filesystem::is_directory(pPath)) {
                std::filesystem::directory_iterator{pPath}; //should fail if not accessable
                return true;
            } else if(std::filesystem::is_regular_file(pPath)) {
                return true;
            } else {
                if(bPrintErrors) {
                    util::qPrint("Error in hasPathPermission! Path:",pPath.c_str(),"is not a directory or regular file!");
                }
                return false;
            }

        } catch (const std::filesystem::filesystem_error& error){
            if(bPrintErrors) {
                util::qPrint("Filesystem error:",error.what());
            }
            return false;
        } catch (...) {
            if(bPrintErrors) {
                util::qPrint("Unknown Error in hasPathPermission!");
            }
            return false;
        }

    }

    std::string vectorToSingleStr ( std::vector<std::string>& sFullVec, bool bAddNewLines ) {
        std::string sOutput = "";

        for(auto& s : sFullVec) {
            sOutput.append(s);
            if(bAddNewLines) { sOutput.push_back('\n'); }
        }

        return sOutput;
    }

    int lerpInt ( int iStart, int iEnd, float fLerp ) {
        return iStart + std::round((float(iEnd) - float(iStart)) * fLerp);
    }


}
