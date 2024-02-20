#include "config.h"

//default config options
std::map<std::string, std::string> config::mDefaultConfig{
    {"theme","Default"},
    {"windowWidth","1600"},
    {"windowHeight","900"},
    {"fontSize","24"},
    {"favorites","/,/home"}
};

bool
    config::bShouldSaveElement = false,
    config::bShouldSaveSystem = false,
    config::bShouldSaveTheme = false;

std::map<std::string, std::string>
    config::mLoadedConfig {},
    config::mLoadedTheme {},
    config::mLoadedElementsInfo {};

std::vector<std::string> config::vFoundThemes;

//this is hardcoded because if the default theme file gets deleted
std::map<std::string,std::string> config::mDefaultNewTheme {
    {"frameRounding","8.000000"}, //should also modify grabRounding & popupRounding
    {"frameBorder","1.000000"},
    {"framePad","14.000000,8.000000"},
    {"itemSpacing","14.000000,8.000000"},
    {"scrollBarSize","14.000000"},
    {"scrollBarRounding","4.000000"},
    {"tabRounding","8.000000"},
    {"circleTess","5.000000"},
    {"separatorThickness","3.000000"},
    {"grabMinSize","30.000000"},
    //~ is the marker for color
    {"~Text","0.000000,0.000000,0.000000,1.000000"},
    {"~TextDisabled","0.100860,0.197749,0.317585,1.000000"},
    {"~WindowBg","0.224452,0.337621,0.545098,1.000000"},
    {"~ChildBg","0.000000,0.000000,0.000000,0.000000"},
    {"~PopupBg","0.192157,0.477278,1.000000,0.968627"},
    {"~Border","0.000000,0.000000,0.000000,1.000000"},
    {"~BorderShadow","0.000000,0.000000,0.000000,0.000000"},
    {"~FrameBg","0.000000,0.362508,0.698163,1.000000"},
    {"~FrameBgHovered","0.000000,0.517647,1.000000,1.000000"},
    {"~FrameBgActive","0.000000,0.317647,0.611765,1.000000"},
    {"~TitleBg","0.194018,0.460280,0.650980,1.000000"},
    {"~TitleBgActive","0.309804,0.615686,1.000000,1.000000"},
    {"~MenuBarBg","0.000000,0.517647,1.000000,0.313726"},
    {"~ScrollbarBg","0.360000,0.310000,1.000000,0.100000"},
    {"~ScrollbarGrab","0.215686,0.234141,1.000000,1.000000"},
    {"~ScrollbarGrabHovered","0.215686,0.307958,1.000000,1.000000"},
    {"~ScrollbarGrabActive","0.215686,0.510957,1.000000,1.000000"},
    {"~CheckMark","0.000000,0.834646,1.000000,1.000000"},
    {"~Button","0.260000,0.590000,0.980000,0.400000"},
    {"~ButtonHovered","0.260000,0.590000,0.980000,1.000000"},
    {"~ButtonActive","0.060000,0.530000,0.980000,1.000000"},
    {"~Separator","0.000000,0.000000,0.000000,1.000000"},
    {"~SeparatorHovered","0.100000,0.400000,0.750000,0.780000"},
    {"~SeparatorActive","0.100000,0.400000,0.750000,1.000000"},
    {"~ResizeGrip","0.260000,0.590000,0.980000,0.200000"},
    {"~ResizeGripHovered","0.260000,0.590000,0.980000,0.670000"},
    {"~ResizeGripActive","0.260000,0.590000,0.980000,0.950000"},
    {"~Tab","0.290980,0.431825,0.623529,1.000000"},
    {"~TabHovered","0.234141,0.485360,0.796078,1.000000"},
    {"~TabActive","0.200000,0.410000,0.680000,1.000000"},
    {"~TabUnfocused","0.070000,0.100000,0.150000,0.970000"},
    {"~TabUnfocusedActive","0.140000,0.260000,0.420000,1.000000"},
    {"~DockingPreview","0.260000,0.590000,0.980000,0.700000"},
    {"~DragDropTarget","1.000000,1.000000,0.000000,0.900000"},
    {"~NavHighlight","0.000000,0.517647,1.000000,1.000000"},
    {"~NavWindowingHighlight","1.000000,1.000000,1.000000,0.700000"},
    {"~NavWindowingDimBg","0.131765,0.374145,0.600000,1.000000"},
    {"~ModalWindowDimBg","0.800000,0.800000,0.800000,0.350000"},
    {"~SliderGrab","0.000000,0.724410,1.000000,1.000000"},
    {"~SliderGrabActive","0.000000,0.944882,1.000000,1.000000"},
    {"~TextSelectedBg","0.000000,0.517647,1.000000,1.000000"},
    {"~Header","0.260000,0.590000,0.980000,0.400000"},
    {"~HeaderHovered","0.260000,0.590000,0.980000,1.000000"},
    {"~HeaderActive","0.060000,0.530000,0.980000,1.000000"},
    //$ is the symbol for customColors
    {"$Warning","1.000000,0.988235,0.000000,1.000000"},
    {"$Critical","0.705882,0.000000,0.000000,1.000000"}

};

/*options to set manually
 * windowMenuButtonPosition = none
 *
*/
config::config() {
    findAllThemes();
    loadConfig(system);
    loadConfig(theme);
    loadConfig(element);
}

//Returns string of "NULL" if property not found
std::string config::getProp(configType cfgFrom, const char* propertyName ) {
    std::map<std::string,std::string> *mConfigGetPtr = &mLoadedConfig;
    std::string sOutput;
    switch(cfgFrom){
        case system:
            //mConfigGetPtr = &mLoadedConfig;
            break;
        case theme:
            mConfigGetPtr = &mLoadedTheme;
            break;
        case element:
            mConfigGetPtr = &mLoadedElementsInfo;
            break;
        default:
            util::qPrint("Attempted to get property value:",propertyName,
                         "from",cfgFrom,"but is not implemented!");
            //mConfigGetPtr = &mLoadedConfig;
            break;
    }

    if( mConfigGetPtr->count(propertyName) == 0) {
        return std::string("NULL");
    } else {
        sOutput = (*mConfigGetPtr)[propertyName];
    }
    return sOutput;
}

void config::update (configType cfgTo, const char* propertyName, std::string sNewValue) {
    //will save config on next tick to limit save calls
    switch(cfgTo){
        case system:
            bShouldSaveSystem = true;
            mLoadedConfig[propertyName] = sNewValue;
            if(std::string(propertyName) == "theme") {
                saveConfig(cfgTo); //force save before load when changing theme
                loadConfig(theme);
            }
            return;
        case theme:
            bShouldSaveTheme = true;
            mLoadedTheme[propertyName] = sNewValue;
            return;
        case element:
            bShouldSaveElement = true;
            mLoadedElementsInfo[propertyName] = sNewValue;
            return;
        default:
            util::qPrint("Attempted to update config type:",cfgTo,"but is not implemented!");
            return;
    }
}

//TODO element here
void config::saveConfig(configType cfgSaveTo) {
    std::string
        cfgLocation,
        sPrintOut;
    switch (cfgSaveTo){
        case system:
            sPrintOut = "System";
            bShouldSaveSystem = false;
            cfgLocation = "../hb.config";
            break;
        case theme:
            sPrintOut = "Theme";
            bShouldSaveTheme = false;
            cfgLocation = getThemePathByName(getProp(system,"theme"));
            break;
        case element:
            sPrintOut = "Element Info";
            bShouldSaveElement = false;
            cfgLocation = "../sys/elements.hbinfo";
            break;
        default:
            util::qPrint("Attempted to save config type:",cfgSaveTo,"but is not implemented!");
            return;
            break;
    }
    if(!(std::filesystem::exists(cfgLocation))) {
        util::qPrint("Config file not found at",cfgLocation,"\n","Attempting to create file...");
        loadConfig(cfgSaveTo);
        return;
    } else if(cfgSaveTo != element) { //config file exists
        std::ifstream fileIn;
        std::ofstream fileOut;
        std::string sLine;
        std::vector<std::string> vFileLines;

        fileIn.open(cfgLocation);

        if(fileIn.is_open()){
            while(getline(fileIn,sLine)) {
                if( sLine.length() == 0) { continue; }
                vFileLines.push_back( sLine );
            }
            setConfigVariableValues(cfgSaveTo, vFileLines );
        } else {
            util::qPrint("Config file was found at",cfgLocation, "but could not be opened!");
            return;
        }

        fileIn.close();
        fileOut.open(cfgLocation,std::ios::out | std::ios::trunc);
        if(fileOut.is_open()){
            for(std::string& ln : vFileLines ){
                fileOut << ln;
                fileOut << "\n";
            }
        }
        fileOut.close();
    } else { //config exists and is element
        std::ofstream fileOut;
        std::vector<std::string> vOutputLines;
        std::string
            sOutput = "",
            sBuild = "";

        for(auto& item : mLoadedElementsInfo) {
            sOutput.append(item.first);
            sOutput.append("{\n");
            sOutput.append(item.second);
            sOutput.append("\n}\n");
        }
        //this makes sure no blank lines will be output
        for(auto& c : sOutput) {
            if(c == '\n') {
                if(sBuild.length() == 0) { continue; }
                vOutputLines.push_back(sBuild);
                sBuild.clear();
            } else { sBuild.push_back(c); }
        }

        fileOut.open(cfgLocation,std::ios::trunc);

        if(fileOut.is_open()) {
            for(auto& ln : vOutputLines) {
                fileOut << ln;
                fileOut << '\n';
            }
        }

        fileOut.close();
    }
    util::qPrint(sPrintOut,"config file saved!");
}

//this does not feel very efficient but maybe I'll make it better when I am
//a better programmer
void config::loadConfig(configType cfgLoadFrom) {
    std::string
        sLine,
        sBuild,
        sProp;
    std::vector<std::string> vFoundProps;
    std::string cfgLoadLocation;
    std::map<std::string,std::string>
        *mConfigLoadPtr = &mLoadedConfig,
        *mDefaultLoadPtr = &mDefaultConfig;

    //TODO should change how the default config gets set
    //maybe create a separate header file with just the default config variables
    switch(cfgLoadFrom){
        case system:
            cfgLoadLocation = "../hb.config";
            //mConfigLoadPtr = &mLoadedConfig;
            //mDefaultPtr = &mDefaultConfig;
            break;
        case theme:
            cfgLoadLocation = getThemePathByName(getProp(system,"theme"));
            mConfigLoadPtr = &mLoadedTheme;
            mDefaultLoadPtr = &mDefaultNewTheme;
            break;
        case element:
            //TODO right now this breaks down if the file is not found
            //need to add default config to new file with rewrite of other defaultConfigs
            cfgLoadLocation = "../sys/elements.hbinfo";
            mConfigLoadPtr = &mLoadedElementsInfo;
            break;
        default:
            util::qPrint("Attempted to load config type:",cfgLoadFrom,"but is not implemented!");
            cfgLoadLocation = "../hb.config";
            //mConfigLoadPtr = &mLoadedConfig;
            //mDefaultLoadPtr = &mDefaultConfig;
            return;
    }

    //create config file if doesn't exist
    //TODO Fix broken element info doesn't exist here
    if (!(std::filesystem::exists(cfgLoadLocation))) {
        std::ofstream fileOut;
        fileOut.open(cfgLoadLocation);
        if(fileOut.is_open()){
            for(auto configOption : *mDefaultLoadPtr ) {
                fileOut << configOption.first;
                fileOut << '=';
                fileOut << configOption.second;
                fileOut << '\n';
            }
            fileOut.close();

            //only time this should be called is when default theme not found,
            //other themes not found reset to Default theme, this makes sure
            //the default theme shows up in the theme selection after created
            if(cfgLoadFrom == theme) {
                vFoundThemes.push_back("Default");
            }
        } else {
            util::qPrint("User config at",cfgLoadLocation,"could not be created!");
        }
    }

    //read file and load config props and values
    std::ifstream fileIn;
    fileIn.open(cfgLoadLocation);
    if(fileIn.is_open()){
        if(cfgLoadFrom == element) {
            bool
                bElementOpen = false,
                bNotesOpen = false,
                bSkipNextNewLine = false;
            sBuild.clear();
            sProp.clear();

            while(getline(fileIn,sLine)) {
                for(char& c : sLine) {
                    if(!bElementOpen) {
                        if(c == '{') {
                            sProp = sBuild;
                            sBuild.clear();
                            bElementOpen = true;
                            bSkipNextNewLine = true;
                        } else {
                            sBuild.push_back(c);
                        }
                    } else { //Element is open
                        if(!bNotesOpen && c == '}') {
                            (*mConfigLoadPtr)[sProp] = sBuild;
                            sProp.clear();
                            sBuild.clear();
                            bElementOpen = false;
                            bSkipNextNewLine = true;
                            continue;
                        }
                        if(c == '{') { bNotesOpen = true; }
                        if(c == '}') { bNotesOpen = false; }
                        sBuild.push_back(c);
                    }
                }
                if(!bSkipNextNewLine) {
                    sBuild.push_back('\n');
                } else { bSkipNextNewLine = false; }

            }
        } else { //is not element
            while(getline(fileIn,sLine )) {
                sProp.clear();
                sBuild.clear();
                if( sLine.length() == 0) { continue; }
                for(char& cElement : sLine ){
                    if( cElement == '='){
                        sProp = sBuild;
                        sBuild.clear();
                    } else {
                        sBuild.push_back( cElement );
                    }
                }
                (*mConfigLoadPtr)[sProp] = sBuild;
                vFoundProps.push_back( sProp );
            }
        }

        fileIn.close();

    } else {
        util::qPrint("User config at", cfgLoadLocation,"could not be opened!");
    }

    //TODO remove this when default configs fixed
    if(cfgLoadFrom == element) return;

    //if any default props did not exist on file
    if(vFoundProps.size() < mDefaultLoadPtr->size()) {
        std::ofstream fileOut;
        fileOut.open(cfgLoadLocation,std::ios::app);
        //add any unfound props from default props
        if(fileOut.is_open()){
            for(auto& configOption : *mDefaultLoadPtr ){
                if(!util::searchVector(vFoundProps,configOption.first)) {
                    fileOut << configOption.first;
                    fileOut << "=";
                    fileOut << configOption.second;
                    fileOut << "\n";
                    (*mConfigLoadPtr)[configOption.first] = configOption.second;
                }
            }
            fileOut.close();
        } else {
            util::qPrint("Config could not be opened when attempting to add additional default props at",cfgLoadLocation);
        }

    }

}

void config::setConfigVariableValues (configType cfgTyp, std::vector<std::string>& vFileLines ) {
    if(cfgTyp == element) {
        util::qPrint("setConfigVariableValues called on Element but should not be called!");
        return;
    }

    std::string sBuild, sNewValue;

    //for each line
    for(std::string& sLine : vFileLines ) {
        sBuild.clear();
        sNewValue.clear();

        for(char& cElement : sLine){
            if( cElement == '=') { //if buildStr has completed
                sNewValue = getProp(cfgTyp,sBuild.c_str());

                //does not exist
                if( sNewValue == std::string("NULL")) {
                    util::qPrint("Property",sBuild,"was not found!");
                    break;
                } else {
                    sLine.erase();
                    sLine.append(sBuild);
                    sLine.append("=");
                    sLine.append(sNewValue);
                    break; //exit for-loop to next line
                }

            } else { sBuild.push_back( cElement ); }
        }
    }
}

std::string config::getThemePathByName(std::string sName) {
    std::string sOutput;
    if(!util::searchVector(vFoundThemes,sName) && sName != "Default"){
        sName = "Default";
        config::update(config::system,"theme",sName);
        //loadConfig(theme);
    }
    sOutput.append("../UI/Themes/");
    sOutput.append(sName);
    sOutput.append(".hbtheme");
    return sOutput;
}

void config::findAllThemes() {
    vFoundThemes.clear();
    for(auto& file : std::filesystem::directory_iterator("../UI/Themes/")){
        if(file.is_regular_file() && (file.path().extension() == ".hbtheme")){
            vFoundThemes.push_back(getThemeNameByPath(file.path()));
        }
    }
}

std::string config::getThemeNameByPath ( std::string sPath ) {
    bool bDotFound = false;
    std::string sBuild;

    //reverse for loop
    for(int i = sPath.length();i>1;i--){
        if(sPath[i] == '.'){
            bDotFound = true;
            continue;
        }
        if(sPath[i] == '/') { return sBuild; }
        if(bDotFound) { sBuild.insert(sBuild.begin(),sPath[i]); }
    }
    util::qPrint("Error unable to getThemeNameByPath with path:",sPath);
    return std::string("Error :(");
}

std::vector<std::string> config::getAllThemeNames() {
    return vFoundThemes;
}

std::map<std::string, std::string> config::getConfig ( configType cfgToGet ) {
    switch(cfgToGet){
        case system:
            return mLoadedConfig;
        case theme:
            return mLoadedTheme;
        case element:
            return mLoadedElementsInfo;
        default:
            util::qPrint("Tried to get config:",cfgToGet,"but is not implemented!");
            return mLoadedConfig;
    }
}

std::map<std::string, std::string> config::getAllThemeColorValues() {
    std::map<std::string,std::string> mOutput;

    for(auto& themeOption : mLoadedTheme) {
        if(themeOption.first[0] == '~') {
            mOutput.insert(themeOption);
        }
    }

    return mOutput;

}

void config::checkIfShouldSaveConfigs() {
    if(bShouldSaveTheme) { saveConfig(theme); }
    if(bShouldSaveSystem) { saveConfig(system); }
    if(bShouldSaveElement) { saveConfig(element); }
}

void config::createNewThemeFromCurrent ( std::string sNewThemeName ) {
    std::string sFilePath = "";
    sFilePath.append("../UI/Themes/");
    sFilePath.append(sNewThemeName);
    sFilePath.append(".hbtheme");

    std::ofstream fileOut;
    //shouldn't need to check if exists because check happens before button can be pressed
    fileOut.open(sFilePath);

    //populate file with theme values
    if(fileOut.is_open()) {
        for(auto& themeElement : mLoadedTheme) {
            fileOut << themeElement.first;
            fileOut << "=";
            fileOut << themeElement.second;
            fileOut << "\n";
        }
    } else {
        util::qPrint("Error creating file",sFilePath);
    }

    fileOut.close();

    vFoundThemes.push_back(sNewThemeName);
    update(system,"theme",sNewThemeName);

}

std::vector<std::string> config::getFavorites() {
    std::vector<std::string> vOutput;

    std::string sProp = getProp(system,"favorites");
    if(sProp == "NULL") { return vOutput; }

    std::string sBuild = "";
    int iPropLength = sProp.length();

    for(int i = 0; i < iPropLength; i++) {
        if(sProp[i] == ',' || i == iPropLength - 1) {
            //this feels slopy to me but I'm tired and it works so I'll fix later
            if(i == iPropLength - 1) { sBuild.push_back(sProp[i]); }
            vOutput.push_back(sBuild);
            sBuild.clear();
        } else { sBuild.push_back(sProp[i]); }
    }

    return vOutput;
}

//this function feels like it's not written very efficiently
void config::modifyFavorites ( std::string sPath, bool bRemove ) {
    std::string sCurrentProp = mLoadedConfig["favorites"];
    int iCurrentPropLength = sCurrentProp.length();
    std::string sBuild = "", sNewProp = "";
    bool bShouldUpdate = true;

    if(bRemove) {
        util::qPrint("Modify favs bRemove true");
        for(int i = 0; i< iCurrentPropLength; i++) {
            if(sCurrentProp[i] == ',' || i == iCurrentPropLength - 1) {
                //this feels slopy to me but I'm tired and it works so I'll fix later
                //...probably
                if(i == iCurrentPropLength - 1) { sBuild.push_back(sCurrentProp[i]); }
                if(sBuild != sPath) {
                    sNewProp.append(sBuild);
                    sNewProp.append(",");
                }
                sBuild.clear();
            } else { sBuild.push_back(sCurrentProp[i]); }
        }
        //remove any duplicate or trailing ','
        int iNewPropLength = sNewProp.length();
        for(int i = iNewPropLength - 1; i > -1; i--) {
            if(i == 0) { break; } //can't check before
            if((sNewProp[i] == ',' && sNewProp[i-1] == ',') //duplicate
            || (i == iNewPropLength - 1 && sNewProp[i] == ',') /*trailing*/ ) {
                sNewProp.erase(sNewProp.begin() + i);
            }
        }
    } else {
        util::qPrint("Modify favs bRemove false");
        //make sure to only add if not already added
        auto vCurrentFavs = util::splitStringOnChar(sCurrentProp,',');
        if(!util::searchVector(vCurrentFavs,sPath)) {
            sNewProp = sCurrentProp;
            sNewProp.append(",");
            sNewProp.append(sPath);
        } else { bShouldUpdate = false; }

    }

    if(bShouldUpdate) { update(system,"favorites",sNewProp); }
}

std::map<std::string, std::string> config::getAllCustomColorValues() {
    std::map<std::string, std::string> mOutput;

    for(auto& item : mLoadedTheme) {
        if(item.first[0] == '$') {
            mOutput.insert(item);
        }
    }

    return mOutput;
}


