#include "html.h"

std::vector<html::file*> html::vLoadedHTMLs;
std::map<std::string,html::elementInfo> html::mElementInfo;
html::elementInfo* html::editElement = NULL;

//html constructor
html::html() {
    auto mConfig = config::getConfig(config::element);
    elementInfo currentElementInfo;
    int iCurrentVariable = 0; //0 bContainer, 1 desc, 2 commonAttributes, 3 notes, 4 link
    bool bCurVarDetermined = false;
    int iLineCount = 0; //used to check if just a full blank line

    for(auto& item : mConfig) {
        currentElementInfo.sName = item.first;
        std::string sBuild = "";
        currentElementInfo.vNotes.clear();

        for(int i = 0; i < item.second.length(); i++) {
            if(!bCurVarDetermined && item.second[i] == '=') {
                switch(sBuild[0]) {
                    case 'f':
                        iCurrentVariable = 0;
                        break;
                    case 'c':
                        iCurrentVariable = 1;
                        break;
                    case 'd':
                        iCurrentVariable = 2;
                        break;
                    case 'a':
                        iCurrentVariable = 3;
                        break;
                    case 'l':
                        iCurrentVariable = 4;
                        break;
                    default:
                        util::qPrint("Error parsing .hbinfo! Expected f|c|d|a but recieved:",sBuild[0]);
                        break;
                }
                bCurVarDetermined = true;
                sBuild.clear();
                continue;
            }

            if(item.second[i] == '\n') {
                if(iLineCount > 0) {
                    if(iCurrentVariable == 5 && sBuild[0] == '}') {
                        mElementInfo[currentElementInfo.sName] = currentElementInfo;
                        iCurrentVariable = 0;
                        bCurVarDetermined = false;
                    } else switch(iCurrentVariable) {
                        case 0:
                            currentElementInfo.sFullName = sBuild;
                            break;
                        case 1:
                            currentElementInfo.bContainer = util::strToInt(sBuild);
                            break;
                        case 2:
                            currentElementInfo.sDescription = sBuild;
                            break;
                        case 3:
                            if(sBuild.length() > 0) {
                                currentElementInfo.vCommonAttributes = util::splitStringOnChar(sBuild,',');
                            } else {
                                currentElementInfo.vCommonAttributes.clear();
                            }
                            break;
                        case 4:
                            currentElementInfo.sWebLink = sBuild;
                            iCurrentVariable++;
                            break;
                        case 5:
                            if(sBuild.length() > 1) {
                                currentElementInfo.vNotes.push_back(sBuild);
                            }
                            break;
                        default:
                            util::qPrint("iCurrentVariable in html constructor out of defined bounds. Value:",iCurrentVariable);
                            break;
                    }
                    sBuild.clear();
                    if(iCurrentVariable != 5) { bCurVarDetermined = false; }
                    iLineCount = 0;
                }
            } else {
                iLineCount++;
                sBuild.push_back(item.second[i]);
            }
        }
    }
}

void html::loadFile ( std::string sFilePath ) {
    //do not open if already open
    for(auto& file : vLoadedHTMLs) {
        if(file->sFileLocation == sFilePath) { return; }
    }

    if(std::filesystem::exists(sFilePath)) {
        vLoadedHTMLs.push_back(new file(sFilePath));
        util::qPrint("Found file",sFilePath);
    } else {
        util::qPrint("Tried to load file at",sFilePath,"but failed!");
    }
}

html::~html() {
    for(auto& file : vLoadedHTMLs) {
        closeFile(file->sFileLocation);
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
            //will delete all children elements of pointer
            delete vLoadedHTMLs[i]->rootElementPtr;
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
    new element(*this,NULL,int2d(-1,sFullRawFile.length() - 1));

}

//constructor element
html::element::element ( file& file, element* parent, int2d startEnd ) {
    //TODO stylePtr and CSS stuff
    parentPtr = parent;
    filePtr = &file;
    indexStartEnd = startEnd;
    iCurrentIndex = indexStartEnd.x;

    bool
        bFoundFirst = false;

    if(!parentPtr) {
        sElementName = "HTML_ROOT";
        filePtr->rootElementPtr = this;

    } else {
        //determine current element name & values
        //TODO stylePtr would be set in here

        //sRawLine setting
        for(int i = iCurrentIndex; i < indexStartEnd.y + 1; i++) {
            if((filePtr->sFullRawFile[i] == ' ' || filePtr->sFullRawFile[i] == '\n')
            && !bFoundFirst
            ) {/*Do nothing*/}
            else if(filePtr->sFullRawFile[i] == '<') {
                bFoundFirst = true;
                bIsElement = true;
                sRawLine.push_back(filePtr->sFullRawFile[i]);
            } else {
                bFoundFirst = true;
                sRawLine.push_back(filePtr->sFullRawFile[i]);
            }

            if(bIsElement && filePtr->sFullRawFile[i] == '>') {
                break;
            }
            iCurrentIndex++;
        }

        if(bIsElement) { populateElementValues(); }
        if(sRawLine.length() == 0
        ||(sRawLine[0] == '<' && sRawLine[1] == '/')
        ) {
            delete this;
            return;
        }

        parentPtr->vChildrenPtrs.push_back(this);
    }

    //this creates an extra empty element at the end, but empty elements get deleted
    int2d next;
    while(iCurrentIndex < indexStartEnd.y) {
        next = getNextElement();
        new element(*filePtr,this,next);
        iCurrentIndex = next.y;
    }
}

//parse thru to get the next
int2d html::element::getNextElement() {
    //y gets incremented before the check in the for loop below
    int2d output = int2d(iCurrentIndex + 1,iCurrentIndex);
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
                output.x = i;
                bFirstCharFound = true;
                bCarrotFound = true;
                sBuild.push_back( cCurrent );
            } else {
                output.x = i;
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

    if(!bNameSet) {
        sElementName = sBuild;
    }
}

html::element::~element() {
    for(auto& element : vChildrenPtrs) {
        if(element) { delete element; }
    }
}

void html::elementInfo::update() {
    std::string sConfigValue = "";

    sConfigValue.append("f=");
    sConfigValue.append(sFullName);
    sConfigValue.append("\nc=");
    sConfigValue.append(std::to_string(bContainer));
    sConfigValue.append("\nd=");
    sConfigValue.append(sDescription);
    sConfigValue.append("\na=");
    sConfigValue.append(util::vectorToSingleStr(vCommonAttributes,std::string(",")));
    sConfigValue.append("\nl=");
    sConfigValue.append(sWebLink);
    sConfigValue.append(std::string("\n{\n"));
    sConfigValue.append(util::vectorToSingleStr(vNotes));
    sConfigValue.append(std::string("}"));

    config::update(config::element,sName.c_str(),sConfigValue);

}

//specifically designed for the Mozilla page
//TODO need to account for the different headers
//h1-h5 don't populate, only h6 does so should be able to just copy h6 for each
void html::parseHtmlForElementInfos ( std::string sMozillaHtml ) {
    std::set<std::string> setSections {
        "aria-labelledby=\"main_root\"",
        "aria-labelledby=\"document_metadata\"",
        "aria-labelledby=\"sectioning_root\"",
        "aria-labelledby=\"content_sectioning\"",
        "aria-labelledby=\"text_content\"",
        "aria-labelledby=\"inline_text_semantics\"",
        "aria-labelledby=\"image_and_multimedia\"",
        "aria-labelledby=\"embedded_content\"",
        "aria-labelledby=\"svg_and_mathml\"",
        "aria-labelledby=\"scripting\"",
        "aria-labelledby=\"demarcating_edits\"",
        "aria-labelledby=\"table_content\"",
        "aria-labelledby=\"forms\"",
        "aria-labelledby=\"interactive_elements\"",
        "aria-labelledby=\"web_components\"",
        "aria-labelledby=\"obsolete_and_deprecated_elements\""
    };

    html::elementInfo elementBuild;

    std::string
        sBuild = "",
        sTDBuild = "";
    bool
        bInsideSection = false,
        bCarrotSearch = false, //I think technically it's not a carrot but meh
        bTDFound = false,
        bElementStarted = false;

    //There is a lot of if statements here, TODO simplify this later
    for(char c : sMozillaHtml) {
        if(bInsideSection) {
            if(bTDFound) {
                if(bCarrotSearch) {
                    sBuild.push_back(c);
                    if(c == '>') {
                        bCarrotSearch = false;
                        if(sBuild == "</td>") {
                            sBuild.clear();
                            bTDFound = false;
                            if(bElementStarted) {
                                santizeDesc(sTDBuild);
                                elementBuild.sDescription = sTDBuild;
                                sTDBuild.clear();
                                bElementStarted = false;
                                if(mElementInfo.find(elementBuild.sName) == mElementInfo.end()) {
                                    mElementInfo.emplace(elementBuild.sName,elementBuild);
                                    elementBuild.update();
                                    elementBuild.clearData();
                                }
                            } else {
                                parseAndSetElementFromHtmlFirstLine(elementBuild,sTDBuild);
                                bElementStarted = true;
                                sTDBuild.clear();
                            }
                        }
                    }
                } else if(c == '<') {
                    bCarrotSearch = true;
                    sTDBuild.append(sBuild);
                    sBuild.clear();
                    sBuild.push_back(c);
                } else sBuild.push_back(c);
            } else if(bCarrotSearch) {
                if(c == '>') {
                    bCarrotSearch = false;
                    if(sBuild == "td") { bTDFound = true; }
                    else if(sBuild == "/section") { bInsideSection = false; }
                    sBuild.clear();
                } else {
                    sBuild.push_back(c);
                }
            } else if(c == '<') {
                bCarrotSearch = true;
            }
        } else {
            if(c == ' ' || c == '>' || c == '\n') {
                if(setSections.find(sBuild) != setSections.end()) { bInsideSection = true; }
                sBuild.clear();
            } else {
                sBuild.push_back(c);
            }
        }
    }

}

void html::elementInfo::clearData() {
    sName.clear();
    sFullName.clear();
    sDescription.clear();
    sWebLink.clear();
    bContainer = true;
    vCommonAttributes.clear();
    vNotes.clear();
}

void html::parseAndSetElementFromHtmlFirstLine ( elementInfo& buildElement, std::string sLine ) {
    std::string sBuild = "";
    bool bBuildStarted = false;

    for(char c : sLine) {
        if(bBuildStarted) {
            if(c == '"') {
                bBuildStarted = false;
                buildElement.sWebLink = "https://developer.mozilla.org";
                buildElement.sWebLink.append(sBuild);
                sBuild.clear();
            } else if (c == '&') {
                bBuildStarted = false;
                buildElement.sName = sBuild;
                buildElement.sFullName = sBuild;
                sBuild.clear();
            } else { sBuild.push_back(c); }
        } else if(c == '"' || c == ';') { bBuildStarted = true; }
    }
}

void html::santizeDesc ( std::string& desc ) {
    bool
        bCarrotSearch = false,
        bCodeBlock = false,
        bAmpersandBuild = false;
    std::string
        sBuild = "",
        sOutput = "";

    for(char c : desc) {
        if(bCarrotSearch) {
            if(c == '>') {
                bCarrotSearch = false;
                if(sBuild == "code" || sBuild == "/code") { util::flip(bCodeBlock); }
                sBuild.clear();
            } else { sBuild.push_back(c); }
        } else if(c == '<') {
            bCarrotSearch = true;
            sBuild.clear();
        } else if(bCodeBlock) {
            if(bAmpersandBuild) {
                if(c == ';') {
                    if(sBuild == "lt") { sOutput.push_back('<'); }
                    else if (sBuild == "gt") { sOutput.push_back('>'); }
                    else util::qPrint("Error when sanitizing Desc: \"&",sBuild,";\" not defined!");
                    sBuild.clear();
                    bAmpersandBuild = false;
                } else { sBuild.push_back(c); }
            } else if(c == '&') {
                bAmpersandBuild = true;
            } else {
                sOutput.push_back(c);
            }
        } else { sOutput.push_back(c); }
    }

    desc = sOutput;
}
