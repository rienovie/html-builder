#pragma once

#include "util.h"
#include <map>
#include <filesystem>
#include <fstream>
#include <vector>

class config {
public:
    static std::string get(char const* propertyName);
    static void update(char const* propertyName, std::string newValue);

    config();
private:
    static std::map<std::string, std::string> defaultConfig;
    static std::map<std::string, std::string> loadedConfig;

    static void loadConfig();
    static void saveConfig();
    static void setConfigVariableValues(std::vector<std::string>& fileLines);
};
