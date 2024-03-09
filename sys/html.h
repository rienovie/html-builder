#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "util.h"
#include <map>
#include <set>
#include "config.h"

using util::int2d;

class html {
public:

    class style;
    class element;
    class file;
    class elementInfo;

    class style {
    public:
        /*
         * This is kinda hard to read so hopefully this makes sense
         *
         * example(css):
         * h1 {color:blue;font-size:12px;}
         *
         * stored as:
         *
         *  1     2      3        4       5
         *  |     |      |        |       |
         *  h1 { color: blue; font-size: 12px;}
         *
         *            1                  2 & 4       3 & 5
         *            |                    |           |                */
        std::map<std::string,std::map<std::string,std::string>> mStyles;

        /*
         * I'm currently putting handling css off right now because this is very complicated
         * and I need a bit more of an application base line before I try to get this to work
         * the way I have pictured in my head. As of right now, the goal is to have the data stored
         * in something like:
         *    map of
         *      <string (cssSourceFileLocation),
         *      to
         *      <vector of similar to mStyles above
         *
         * I need a vector because I plan of having a list of all the overwrites for the css style
         * with file line specified where each overwrite happens for a specific element, but also
         * will probably need the mStyles subMap of the prop/value definitions to be a vector as well
         * so the user will be able to catch any duplicated definitions
         */



    private:

    };

    class element {
    public:
        std::string
            sRawLine, //contains what's in the <> of the element declaration
            sElementName = "Raw"; //Only changed if actual element
        style* stylePtr;
        element* parentPtr;
        file* filePtr;
        bool
            bIsElement = false,
            bIsContainer = false;
        std::vector<element*> vChildrenPtrs;
        std::map<std::string,std::string> mAttributes;

        element(file& file, element* parent, int2d startEnd);
        ~element();
    private:
        int2d indexStartEnd; //this is from the sFullRawFile & only used in construction
        int iCurrentIndex;

        int2d getNextElement();
        void populateElementValues();

    };

    class file {
    public:
        std::string
            sFileLocation,
            sFileName,
            sFullRawFile;
        std::vector<std::string> vFileLines;
        element* rootElementPtr;
        std::set<std::string> setTerminations;

        file(std::string sFilePath);
    private:
        void
            populateFileLines(),
            populateElements();
    };

    class elementInfo {
    public:
        std::string
            sName,
            sFullName,
            sDescription;
        bool bContainer = true;
        std::vector<std::string>
            vCommonAttributes,
            vNotes;

        void update();
    private:

    };

    //public for html class
    static std::vector<file*> vLoadedHTMLs;
    static std::map<std::string,elementInfo> mElementInfo;
    static elementInfo* editElement;

    static void
        loadFile(std::string sFilePath),
        closeFile(std::string sLoadedFileFullPath),
        parseHtmlForElementInfos(std::string sMozillaHtml);

    html();
    ~html();
private:

};
