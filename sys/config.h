#pragma once

#include "../Util/util.hpp"
#include <map>
#include <filesystem>
#include <fstream>
#include <vector>

class config {
public:
    enum configType {
        system,
        theme,
        element
    };

    //variables
    static std::vector<std::string> vFoundThemes;

    //functions
    static void
        removeProp(configType cfgTo,char const* propertyName),
        update(configType cfgTo, char const* propertyName, std::string sNewValue),
        checkIfShouldSaveConfigs(),
        createNewThemeFromCurrent(std::string sNewThemeName),
        findAllThemes(),
        modifyFavorites(std::string sPath,bool bRemove = false),
        deleteCurrentTheme();

    static std::vector<std::string>
        getFavorites(),
        getAllThemeNames();

    static std::map<std::string,std::string>
        getConfig(configType cfgToGet),
        getAllThemeColorValues(),
        getAllCustomColorValues();

    static std::string getProp(configType cfgFrom, const char* propertyName);

    config();

private:
    //variables
    static std::map<std::string, std::string>
        mDefaultConfig,
        mLoadedConfig,
        mLoadedTheme,
        mDefaultNewTheme,
        mLoadedElementsInfo;

    static bool
        bShouldSaveSystem,
        bShouldSaveElement,
        bShouldSaveTheme;

    //functions
    static void
        loadConfig(configType cfgLoadFrom),
        saveConfig(configType cfgSaveTo),
        setConfigVariableValues(configType cfgtyp, std::vector<std::string>& vFileLines);

    static std::string
        getThemePathByName(std::string sName),
        getThemeNameByPath(std::string sPath);

};
