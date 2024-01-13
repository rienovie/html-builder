#include "html.h"

std::vector<html::file*> html::vLoadedHTMLs;

void html::loadFile ( std::string sFilePath ) {
    if(std::filesystem::exists(sFilePath)) {
        file *newFilePTR = new file(sFilePath);
        vLoadedHTMLs.push_back(newFilePTR);
        util::qPrint("Found file",sFilePath);
    } else {
        util::qPrint("Tried to load file at",sFilePath,"but failed!");
    }
}

//destructor
html::~html() {
    for (auto item : vLoadedHTMLs) {
        delete item;
    }
}

//constructor
html::file::file ( std::string sFilePath ) {
    sFileLocation = sFilePath;
    sFileName = "";

    for(int i = sFilePath.length() - 1; i > -1; i--) {
        if(sFilePath[i] == '/') { break; }
        else {
            sFileName.insert(sFileName.begin(),1,sFilePath[i]);
        }
    }

    std::ifstream fileIn;
    std::string sLine;

    fileIn.open(sFilePath);
    if(fileIn.is_open()) {
        while(getline(fileIn,sLine)) {
            vFileLines.push_back(sLine);
        }
    }
    fileIn.close();
}