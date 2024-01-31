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

    bool bIsElement = false;

    if(!parentPtr) {
        sElementName = "HTML";
        filePtr->vElementPtrs.push_back(this);

    } else {
        //determine current element name & values
        //TODO stylePtr would be set in here

        //sRawLine setting
        for(int i = iCurrentIndex; i < indexStartEnd.y + 1; i++) {
            if(filePtr->sFullRawFile[i] == ' '
            || filePtr->sFullRawFile[i] == '\n'
            ) {/*Do nothing*/}
            else if(filePtr->sFullRawFile[i] == '<') {
                bIsElement = true;
                sRawLine.push_back(filePtr->sFullRawFile[i]);
            } else {
                sRawLine.push_back(filePtr->sFullRawFile[i]);
            }

            iCurrentIndex++;
            if(bIsElement && filePtr->sFullRawFile[i] == '>') {
                break;
            }
        }

        if(bIsElement) { populateElementValues(); }
    }

    //this might create a second extra element at the end, TODO should test
    ImVec2 next;
    while(iCurrentIndex < indexStartEnd.y) {
        next = getNextElement();
        new element(*filePtr,this,next);
        iCurrentIndex = next.y;
    }
}

//parse thru to get the next
ImVec2 html::element::getNextElement() {
    //y gets incremented before the check in the for loop below
    ImVec2 output = ImVec2(iCurrentIndex + 1,iCurrentIndex);
    std::string
        sBuild = "",
        sBuiltName = "";
    char cCurrent;
    int iTerminating = 0; // 0 not determined, 1 not terminating, 2 terminating
    bool
        bFirstCharFound = false,
        bCarrotFound = false;

    for(int i = output.x; i <= indexStartEnd.y; i++) {
        cCurrent = filePtr->sFullRawFile[i];
        output.y++;

        if(!bFirstCharFound) {
            if( cCurrent == ' ' || cCurrent == '\n') {/*Do Nothing*/}
            else if( cCurrent == '<'){
                bFirstCharFound = true;
                bCarrotFound = true;
                sBuild.push_back( cCurrent );
            } else {
                bFirstCharFound = true;
                sBuild.push_back( cCurrent );
            }
        } else if (iTerminating < 1) { //first char found and term not determined
            if(bCarrotFound) { //check
                sBuild.push_back(cCurrent);
                if(cCurrent == '>' || cCurrent == ' ') {
                    //TODO check if this breaks with <>
                    sBuiltName = sBuild.substr(1,sBuild.length() - 2);

                    iTerminating = (filePtr->setTerminations.find(sBuiltName) != filePtr->setTerminations.end()) + 1;
                }
            } else { //first char found, term not determined, carrot not found
                //just need to find end for Raw text
                if(cCurrent == '<') {
                    output.y--; //don't want to include this char
                    break;
                }
            }
        } else { //first char found, carrotFound, term determined
            if(iTerminating == 2) {
                if(cCurrent == '<' && filePtr->sFullRawFile[i + 1] == '/') {
                    std::string sCompare = "";

                    for(int j = 0;j<sBuiltName.length();j++) {
                        sCompare.push_back(filePtr->sFullRawFile[i+2+j]);
                    }

                    if(sCompare == sBuiltName) {
                        output.y += sBuiltName.length() + 2; //TODO double check math
                        break;
                    }
                }
            } else {
                if(cCurrent == '>') {
                    break;
                }
            }
        }
    }

    return output;
}

void html::element::populateElementValues() {
    std::string
        sBuild = "",
        sProp = "",
        sValue = "";
    bool
        bNameSet = false,
        bOpenQuotes = false;

    for(int i = 1; i < sRawLine.length() - 1; i++) { //skip first and last
        if(!bOpenQuotes) {
            if (sRawLine[i] == ' ' && !bNameSet) {
                sElementName = sBuild;
                bNameSet = true;
                sBuild.clear();
            } else if (sRawLine[i] == '=') {
                sProp = sBuild;
                sBuild.clear();
            } else if (sRawLine[i] == '"') {
                bOpenQuotes = true;
            } else {
                sBuild.push_back(sRawLine[i]);
            }
        } else { //bOpenQuotes true
            if(sRawLine[i] == '"') {
                sValue = sBuild;
                mAttributes[sProp] = sValue;
                sProp.clear();
                sValue.clear();
                sBuild.clear();
                bOpenQuotes = false;
            } else {
                sBuild.push_back(sRawLine[i]);
            }
        }
    }
}
