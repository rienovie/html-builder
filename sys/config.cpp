#include "config.h"

//default config options
std::map<std::string, std::string> config::mDefaultConfig{
    {"theme","Default"},
    {"windowWidth","1600"},
    {"windowHeight","900"},
    {"fontSize","24"}
};
bool config::bShouldSaveCfg = false;
std::map<std::string, std::string> config::mLoadedConfig{};
std::vector<std::string> config::vFoundThemes;
std::map<std::string, std::string> config::mLoadedTheme {};

//currently this is hardcoded, but I'm not sure why I did this at the start
//TODO replace this monstrosity with building this map from a file that cannot be
//modified; most likely will make the "Default" theme file not be editable from the user
std::map<std::string,std::string> config::mDefaultNewTheme {
    {"frameRounding","4.000000"}, //should also modify grabRounding & popupRounding
    {"frameBorder","2.000000"},
    {"framePad","13.000000,6.000000"},
    {"itemSpacing","14.000000,4.000000"},
    {"scrollBarSize","14"},
    {"scrollBarRounding","4.000000"},
    {"tabRounding","8.000000"},
    {"circleTess","5.000000"},
    {"separatorThickness","1.500000"},
    //~ is the marker for color
    {"~Text","1.00,1.00,1.00,1.00"},
    {"~TextDisabled","0.50,0.50,0.50,1.00"},
    {"~WindowBg","0.00,0.04,0.11,0.94"},
    {"~ChildBg","0.000000,0.000000,0.000000,0.000000"},
    {"~PopupBg","0.000000,0.050000,0.140000,0.970000"},
    {"~Border","1.000000,1.000000,1.000000,0.230000"},
    {"~BorderShadow","0.000000,0.000000,0.000000,0.000000"},
    {"~FrameBg","0.360000,0.310000,1.000000,0.200000"},
    {"~FrameBgHovered","0.310000,0.390000,1.000000,0.620000"},
    {"~FrameBgActive","0.310000,0.390000,1.000000,1.000000"},
    {"~TitleBg","0.07,0.06,0.20,1.00"},
    {"~TitleBgActive","0.36,0.31,1.00,0.22"},
    {"~TitleBgCollapsed","0.00,0.00,0.00,0.50"},
    {"~MenuBarBg","0.000000,0.000000,0.000000,1.000000"},
    {"~ScrollbarBg","0.36,0.31,1.00,0.10"},
    {"~ScrollbarGrab","0.17,0.22,0.55,1.00"},
    {"~ScrollbarGrabHovered","0.22,0.28,0.71,1.00"},
    {"~ScrollbarGrabActive","0.31,0.39,1.00,1.00"},
    {"~CheckMark","0.310000,0.390000,1.000000,1.000000"},
    {"~Button","0.26,0.59,0.98,0.40"},
    {"~ButtonHovered","0.26,0.59,0.98,1.00"},
    {"~ButtonActive","0.06,0.53,0.98,1.00"},
    {"~Separator","0.31,0.39,1.00,1.00"},
    {"~SeparatorHovered","0.31,0.39,1.00,1.00"},
    {"~SeparatorActive","0.31,0.39,1.00,1.00"},
    {"~ResizeGrip","0.31,0.39,1.00,1.00"},
    {"~ResizeGripHovered","0.31,0.39,1.00,1.00"},
    {"~ResizeGripActive","0.31,0.39,1.00,1.00"},
    {"~Tab","0.31,0.39,1.00,1.00"},
    {"~TabHovered","0.31,0.39,1.00,1.00"},
    {"~TabActive","0.31,0.39,1.00,1.00"},
    {"~TabUnfocused","0.31,0.39,1.00,1.00"},
    {"~TabUnfocusedActive","0.31,0.39,1.00,1.00"},
    {"~DockingPreview","0.31,0.39,1.00,1.00"},
    {"~DragDropTarget","0.31,0.39,1.00,1.00"},
    {"~NavHighlight","0.31,0.39,1.00,1.00"},
    {"~NavWindowingHighlight","0.31,0.39,1.00,1.00"},
    {"~NavWindowingDimBg","0.31,0.39,1.00,1.00"},
    {"~ModalWindowDimBg","0.31,0.39,1.00,1.00"},
    {"~SliderGrab","0.26,0.59,0.98,0.40"},
    {"~SliderGrabActive","0.06,0.53,0.98,1.00"},
    {"~TextSelectedBg","0.26,0.59,0.98,1.00"},

};

/*options to set manually
 * windowMenuButtonPosition = none
 *
*/
config::config() {
    findAllThemes();
    loadConfig(system);
    loadConfig(theme);
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
        default:
            util::qPrint("Attempted to get property value at",cfgFrom,"but is not implemented!");
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
    bShouldSaveCfg = true;
    //will save config on next tick to limit save calls
    switch(cfgTo){
        case system:
            mLoadedConfig[propertyName] = sNewValue;
            if(std::string(propertyName) == "theme") {
                saveConfig(cfgTo); //force save before load when changing theme
                loadConfig(theme);
            }
            break;
        case theme:
            mLoadedTheme[propertyName] = sNewValue;
            break;
        default:
            util::qPrint("Attempted to update config type:",cfgTo,"but is not implemented!");
            break;
    }
}

void config::saveConfig(configType cfgSaveTo) {
    std::string cfgLocation;
    switch (cfgSaveTo){
        case system:
            cfgLocation = "../hb.config";
            break;
        case theme:
            cfgLocation = getThemePathByName(getProp(system,"theme"));
            break;
        default:
            util::qPrint("Attempted to save config type:",cfgSaveTo,"but is not implemented!");
            break;
    }
    if(!(std::filesystem::exists(cfgLocation))) {
        util::qPrint("Config file not found at",cfgLocation,"\n","Attempting to create file...");
        loadConfig(cfgSaveTo);
        return;
    } else { //config file exists
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
            for(std::string ln : vFileLines ){
                fileOut << ln;
                fileOut << "\n";
            }
        }
        fileOut.close();
    }
    util::qPrint("Config file saved!");
}

//this does not feel very efficient but maybe I'll make it better when I am
//a better propgramer
void config::loadConfig(configType cfgLoadFrom) {
    std::string sLine, sBuild, sProp;
    std::vector<std::string> vFoundProps;
    std::string cfgLoadLocation;
    std::map<std::string,std::string>
        *mConfigLoadPtr = &mLoadedConfig,
        *mDefaultLoadPtr = &mDefaultConfig;

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
        default:
            util::qPrint("Attempted to load config type:",cfgLoadFrom,"but is not implemented!");
            cfgLoadLocation = "../hb.config";
            //mConfigLoadPtr = &mLoadedConfig;
            //mDefaultLoadPtr = &mDefaultConfig;
            break;
    }

    //create config file if doesn't exist
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
        } else {
            util::qPrint("User config at",cfgLoadLocation,"could not be created!");
        }
    }

    //read file and load config props and values
    std::ifstream fileIn;
    fileIn.open(cfgLoadLocation);
    if(fileIn.is_open()){
        while(getline(fileIn,sLine )) {
            sProp.clear();
            sBuild.clear();
            if( sLine.length() == 0) { continue; }
            for(char cElement : sLine ){
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

        fileIn.close();

    } else {
        util::qPrint("User config at", cfgLoadLocation,"could not be opened!");
    }

    //if any default props did not exist on file
    if(vFoundProps.size() < mDefaultLoadPtr->size()) {
        std::ofstream fileOut;
        fileOut.open(cfgLoadLocation,std::ios::app);
        //add any unfound props from default props
        if(fileOut.is_open()){
            for(auto configOption : *mDefaultLoadPtr ){
                if(!util::searchVector(vFoundProps,configOption.first)) {
                    fileOut << configOption.first;
                    fileOut << "=";
                    fileOut << configOption.second;
                    fileOut << "\n";
                }
            }
            fileOut.close();
        } else {
            util::qPrint("Config could not be opened when attempting to add additional default props at",cfgLoadLocation);
        }

    }

}

void config::setConfigVariableValues (configType cfgTyp, std::vector<std::string>& vFileLines ) {
    std::string sBuild, sNewValue;

    //for each line
    for(std::string& sLine : vFileLines ) {
        sBuild.clear();
        sNewValue.clear();

        for(char cElement : sLine){
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
    if(!util::searchVector(vFoundThemes,sName)){
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
        default:
            util::qPrint("Tried to get config:",cfgToGet,"but is not implemented!");
            return mLoadedConfig;
    }
}

std::map<std::string, std::string> config::getAllThemeColorValues() {
    std::map<std::string,std::string> mOutput;

    for(auto themeOption : mLoadedTheme) {
        if(themeOption.first[0] == '~') {
            mOutput.insert(themeOption);
        }
    }

    return mOutput;

}

void config::checkIfShouldSaveConfigs() {
    if(bShouldSaveCfg) {
        bShouldSaveCfg = false;
        saveConfig(system);
        saveConfig(theme);
    }
}



