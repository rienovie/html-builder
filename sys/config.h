#pragma once

#include "util.h"
#include <map>
#include <filesystem>
#include <fstream>
#include <vector>

class config {
public:
    std::string get(char const* propertyName);
    void update(char const* propertyName, std::string newValue);

    config();
private:
    static std::map<std::string, std::string> defaultConfig;
    static std::map<std::string, std::string> loadedConfig;

    void loadConfig();
    void saveConfig();
};
