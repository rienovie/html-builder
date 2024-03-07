#pragma once

#include <curl/curl.h>
#include <string>
#include <map>
#include "util.h"
#include <thread>
#include <queue>

class fetch {
public:
    enum fetchStatus {
        notActive,
        init,
        fetching,
        fetchSuccess,
        error
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
    static std::queue<fetchData*> pendingFetches;

    static void threadFunc();
};
