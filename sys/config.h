#pragma once

#include "util.h"
#include <map>
#include <filesystem>
#include <fstream>
#include <vector>

class config {
public:
    enum configType {
        system,
        theme
    };

    static std::vector<std::string> vFoundThemes;

    static std::string getProp(configType cfgFrom, const char* propertyName);
    static std::map<std::string,std::string> getConfig(configType cfgToGet);
    static void update(configType cfgTo, char const* propertyName, std::string sNewValue);
    static std::vector<std::string> getAllThemeNames();
    static std::map<std::string,std::string> getAllThemeColorValues();
    static void checkIfShouldSaveConfigs();
    static void createNewThemeFromCurrent(std::string sNewThemeName);
    static void findAllThemes();
    static std::vector<std::string> getFavorites();
    static void modifyFavorites(std::string sPath,bool bRemove = false);

    config();

private:
    static std::map<std::string, std::string> mDefaultConfig;
    static std::map<std::string, std::string> mLoadedConfig;
    static std::map<std::string,std::string> mLoadedTheme;
    static std::map<std::string,std::string> mDefaultNewTheme;
    static bool bShouldSaveSystem, bShouldSaveTheme;

    static void loadConfig(configType cfgLoadFrom);
    static void saveConfig(configType cfgSaveTo);
    static void setConfigVariableValues(configType cfgtyp, std::vector<std::string>& vFileLines);
    static std::string getThemePathByName(std::string sName);
    static std::string getThemeNameByPath(std::string sPath);

};
