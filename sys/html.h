#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "util.h"
#include <map>

/*current theory I'm working with is to have a large html class that has all the elements,
 * current vector of loaded files, and lookup stuff, while having a subclass specific for
 * each file that would be used to modify the html file and data on it
 *
 * I need to rework this theory to have a better architecture for the program
*/

class html {
public:

    class style {
    public:
        /*
         * This is kinda hard to read so hopefully this makes sense
         *
         * example(css):
         * h1 {color:blue;font-size:12px;}
         *
         * stored as:
         * 1 = h1
         * 2 = color
         * 3 = blue
         * 4 = font-size
         * 5 = 12px
                     \/ 1              \/ 2 & 4    \/ 3 & 5   */
        std::map<std::string,std::map<std::string,std::string>> mStyles;
    private:

    };

    class element {
    public:
        std::string sRaw, sElementName;
        style* stylePtr;
        element* parentPtr;
        std::vector<element*> vChildrenPtrs;
        std::map<std::string,std::string> mAttributes;
    private:

    };

    class file {
    public:
        std::string sFileLocation, sFileName;
        std::vector<std::string> vFileLines;

        file(std::string sFilePath);
    private:

    };

    //public for html class
    static std::vector<file*> vLoadedHTMLs;

    static void loadFile(std::string sFilePath);
    static void closeFile(std::string sLoadedFileFullPath);

    ~html();
private:

};
