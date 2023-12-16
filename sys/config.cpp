#include "config.h"

config::config() {
    loadConfig();
}

/*Returns string of "0" if property not found*/
std::string config::get ( const char* propertyName ) {
    if(loadedConfig.count(propertyName) == 0){
        return std::string("0");
    }
    return loadedConfig[propertyName];
}

void config::update ( const char* propertyName, std::string newValue ) {
    loadedConfig[propertyName] = newValue;
    saveConfig();
}

void config::saveConfig() {
    if(!std::filesystem::exists("hb.config")){
        util::qPrint("Config file not found!","\n","Attempting to create file...");
        loadConfig();
        return;
    } else {
        std::fstream file;
        std::string line, buildStr;
        std::vector<std::string> fileLines;

        file.open("hb.config");
        if(file.is_open()){
            while(getline(file,line)){
                fileLines.push_back(line);
            }
            //for each line
            for(std::string lineStr : fileLines){
                buildStr.clear();

                for(char element : lineStr){
                    if(element == '='){
                        if(get(buildStr.c_str()) == std::string("0")){
                            //TODO currently working here
                        }
                    }
                }
            }

        } else {
            util::qPrint("Config file was found but could not be opened!");
            return;
        }

    }
}


void config::loadConfig() {
    std::string line, buildStr , property;

    //create config file if doesn't exist
    if (!std::filesystem::exists("hb.config")) {
        std::ofstream file;
        file.open("hb.config");
        if(file.is_open()){

            for(auto configOption : defaultConfig){
                file << configOption.first;
                file << '=';
                file << configOption.second;
                file << '\n';
            }
            file.close();
        } else {
            util::qPrint("User config could not be created!");
        }
    }

    std::ifstream file;
    file.open("hb.config");
    if(file.is_open()){
        while(getline(file,line)){
            property.clear();
            buildStr.clear();

            for(char element : line){
                if(element == '='){
                    property = buildStr;
                    buildStr.clear();
                } else {
                    buildStr.push_back(element);
                }
            }

        }
        loadedConfig[property] = buildStr;

    } else {
        util::qPrint("User config could not be opened!");
    }
}
