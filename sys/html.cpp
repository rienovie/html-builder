#include "html.h"

std::vector<html::file*> html::vLoadedHTMLs;

void html::loadFile ( std::string sFilePath ) {
    //do not open if already open
    for(auto& file : vLoadedHTMLs) {
        if(file->sFileLocation == sFilePath) { return; }
    }

    if(std::filesystem::exists(sFilePath)) {
        file *newFilePTR = new file(sFilePath);
        vLoadedHTMLs.push_back(newFilePTR);
        util::qPrint("Found file",sFilePath);
    } else {
        util::qPrint("Tried to load file at",sFilePath,"but failed!");
    }
}

html::~html() {
    for (auto& item : vLoadedHTMLs) {
        delete item;
    }
}

//constructor file
html::file::file ( std::string sFilePath ) {
    sFileLocation = sFilePath;
    sFileName = "";

    for(int i = sFilePath.length() - 1; i > -1; i--) {
        if(sFilePath[i] == '/') { break; }
        else {
            sFileName.insert(sFileName.begin(),1,sFilePath[i]);
        }
    }

    populateFileLines();
    sFullRawFile = util::vectorToSingleStr(vFileLines);
    populateElements();
}

void html::closeFile ( std::string sLoadedFileFullPath ) {
    int iSize = vLoadedHTMLs.size();

    for(int i = 0;i < iSize;i++){
        if(vLoadedHTMLs[i]->sFileLocation == sLoadedFileFullPath) {
            delete vLoadedHTMLs[i];
            vLoadedHTMLs.erase(vLoadedHTMLs.begin() + i);
            return;
        }
    }
}

void html::file::populateFileLines() {
    std::ifstream fileIn;
    std::string sLine;

    fileIn.open(sFileLocation);
    if(fileIn.is_open()) {
        while(getline(fileIn,sLine)) {
            vFileLines.push_back(sLine);
        }
    }
    fileIn.close();
}

void html::file::populateElements() {
    bool bElementSearch = false;
    int
        iFileLineSize = vFileLines.size(),
        iSize = 0,
        iElementStartIndex = -1;
    std::string
        sCurrentLine = "",
        sBuild = "";

    //finds all terminating elements
    for(int i = 0; i < iFileLineSize; i++) {
        iSize = vFileLines[i].size();
        sCurrentLine = vFileLines[i];
        sBuild.clear();

        for(int j = 0; j < iSize; j++) {
            if(bElementSearch && iElementStartIndex != j-1) {
                if(sCurrentLine[j] == ' '
                || sCurrentLine[j] == '>'
                || sCurrentLine[j] == '\n'
                ) {
                    bElementSearch = false;
                    if(sBuild.length() != 0) { setTerminations.insert(sBuild); }
                } else {
                    sBuild.push_back(sCurrentLine[j]);
                }
            } else if (sCurrentLine[j] == '<'
                && iSize - 1 != j+1
                && sCurrentLine[j+1] == '/'
            ) {
                bElementSearch = true;
                iElementStartIndex = j;
                sBuild.clear();
            }
        }

    }

    //each element populates it's children when created
    element* rootElem = new element(*this,NULL,ImVec2(0,sFullRawFile.length() - 1));

}

//TODO working in this whole section right now
//constructor element
html::element::element ( file& file, element* parent, ImVec2 startEnd ) {
    //TODO stylePtr and CSS stuff
    parentPtr = parent;
    filePtr = &file;
    indexStartEnd = startEnd;
    iCurrentIndex = indexStartEnd.x;

    if(!parentPtr) {
        sElementName = "HTML";
        filePtr->vElementPtrs.push_back(this);

    } else {

    }

    //this might create a second extra element at the end, TODO should test
    ImVec2 next;
    while(iCurrentIndex != indexStartEnd.y) {
        next = getNextElement();
        element* nextElem = new element(*filePtr,this,next);
        iCurrentIndex = next.y;
    }
}

//parse thru to get the next
ImVec2 html::element::getNextElement() {
    ImVec2 output = ImVec2(iCurrentIndex,iCurrentIndex);
    std::string sBuild = "";
    bool bCarrotFound = false;

    //find < or if just raw(text)

    //determine end point by incrementing output.y
    //if is in terminating set walk to next </thisElement
    //else walk to next < or (> if carrotFound)

    return output;
}

