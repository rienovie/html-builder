#include "config.h"



//default config options
std::map<std::string, std::string> config::defaultConfig{
    {"theme","Default"},
    {"windowWidth","1600"},
    {"windowHeight","900"}
};
std::map<std::string, std::string> config::loadedConfig{};

config::config() {
    loadConfig();
}

//Returns string of "0" if property not found
std::string config::get ( const char* propertyName ) {
    if(loadedConfig.count(propertyName) == 0) {
        return std::string("0");
    }
    return loadedConfig[propertyName];
}

void config::update ( const char* propertyName, std::string newValue ) {
    loadedConfig[propertyName] = newValue;
    saveConfig();
}

void config::saveConfig() {
    if(!(std::filesystem::exists("../hb.config"))) {
        util::qPrint("Config file not found!","\n","Attempting to create file...");
        loadConfig();
        return;
    } else { //config file exists
        std::ifstream fileIn;
        std::ofstream fileOut;
        std::string line;
        std::vector<std::string> fileLines;

        fileIn.open("../hb.config");

        if(fileIn.is_open()){
            while(getline(fileIn,line)) {
                if(line.length() == 0) { continue; }
                fileLines.push_back(line);
            }
            setConfigVariableValues(fileLines);
        } else {
            util::qPrint("Config file was found but could not be opened!");
            return;
        }

        fileIn.close();
        fileOut.open("../hb.config",std::ios::out | std::ios::trunc);
        if(fileOut.is_open()){
            for(std::string line : fileLines){
                fileOut << line;
                fileOut << "\n";
            }
        }
        fileOut.close();
    }
}

//this does not feel very efficient but maybe I'll make it better when I am
//a better propgramer
void config::loadConfig() {
    std::string line, buildStr, property;
    std::vector<std::string> vFoundProps;

    //create config file if doesn't exist
    if (!(std::filesystem::exists("../hb.config"))) {
        util::qPrint("file is not found attempt");
        std::ofstream fileOut;
        fileOut.open("../hb.config");
        if(fileOut.is_open()){
            for(auto configOption : defaultConfig){
                fileOut << configOption.first;
                fileOut << '=';
                fileOut << configOption.second;
                fileOut << '\n';
            }
            fileOut.close();
        } else {
            util::qPrint("User config could not be created!");
        }
    }

    //read file and load config props and values
    std::ifstream fileIn;
    fileIn.open("../hb.config");
    if(fileIn.is_open()){
        while(getline(fileIn,line)) {
            property.clear();
            buildStr.clear();
            if(line.length() == 0) { continue; }
            for(char element : line){
                if(element == '='){
                    property = buildStr;
                    buildStr.clear();
                } else {
                    buildStr.push_back(element);
                }
            }
            loadedConfig[property] = buildStr;
            vFoundProps.push_back(property);
        }

        fileIn.close();

    } else {
        util::qPrint("User config could not be opened!");
    }

    //if any default props did not exist on file
    if(vFoundProps.size() < defaultConfig.size()) {
        std::ofstream fileOut;
        fileOut.open("../hb.config",std::ios::app);
        //add any unfound props from default props
        if(fileOut.is_open()){
            for(auto configOption : defaultConfig){
                if(!util::searchVector(vFoundProps,configOption.first)) {
                    fileOut << configOption.first;
                    fileOut << "=";
                    fileOut << configOption.second;
                    fileOut << "\n";
                }
            }
            fileOut.close();
        } else {
            util::qPrint("Config could not be opened when attempting to add additional default props");
        }

    }

}

void config::setConfigVariableValues ( std::vector<std::string>& fileLines ) {
    std::string sBuild, sNewValue;

    //for each line
    for(std::string& sLine : fileLines) {
        sBuild.clear();
        sNewValue.clear();

        for(char element : sLine){
            if(element == '=') { //if buildStr has completed
                sNewValue = get(sBuild.c_str());

                //does not exist
                if( sNewValue == std::string("0")) {
                    util::qPrint("Property",sBuild,"was not found!");
                    break;
                } else {
                    sLine.erase();
                    sLine.append(sBuild);
                    sLine.append("=");
                    sLine.append(sNewValue);
                    break; //exit for-loop to next line
                }

            } else { sBuild.push_back(element); }
        }
    }
}

