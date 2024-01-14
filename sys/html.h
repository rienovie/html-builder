#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "util.h"

/*current theory I'm working with is to have a large html class that has all the elements,
 * current vector of loaded files, and lookup stuff, while having a subclass specific for
 * each file that would be used to modify the html file and data on it
*/

class html {
public:

    class file {
    public:
        std::string sFileLocation, sFileName;
        std::vector<std::string> vFileLines;

        file(std::string sFilePath);
    private:
    };

    //public for html class
    // I would have put the file class below this for visual clarity but it needs
    // to be put above for the declarations below
    static std::vector<file*> vLoadedHTMLs;

    static void loadFile(std::string sFilePath);
    static void closeFile(std::string sLoadedFileFullPath);

    ~html();
private:

};
