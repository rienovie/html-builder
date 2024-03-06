#pragma once

#include <curl/curl.h>
#include <string>
#include <map>
#include "util.h"
#include <thread>
#include <vector>

class fetch {
public:
    enum fetchStatus {
        notActive,
        init,
        curlInit,
        fetching,
        fetchSuccess,
        curlClean,
        completed
    };

    struct fetchData {
        fetchStatus currentStatus = notActive;
        bool bError = false;
        std::string
            sUrl,
            sHtml,
            sError;

        fetchData(std::string& url) : sUrl(url) {} ;
    };

    static std::map<std::string,fetchData> mLoadedHtmls;

    static fetchData& fetchUrl(std::string sUrl);
private:
    static void threadFunc(fetchData& data);
};
