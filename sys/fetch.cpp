#include "fetch.h"

std::map<std::string,fetch::fetchData> fetch::mLoadedHtmls;
std::queue<fetch::fetchData*> fetch::pendingFetches;

fetch::fetchData& fetch::fetchUrl ( std::string sUrl ) {
    if(mLoadedHtmls.find(sUrl) != mLoadedHtmls.end()) {
        return mLoadedHtmls.at(sUrl);
    }
    mLoadedHtmls.emplace(sUrl,fetchData(sUrl));
    fetchData* dataPtr = &mLoadedHtmls.at(sUrl);

    dataPtr->currentStatus = init;

    if(pendingFetches.empty()) {
        pendingFetches.push(dataPtr);
        std::thread fetchThread(threadFunc);
    } else {
        pendingFetches.push(dataPtr);
    }

    return *dataPtr;
}

void fetch::threadFunc () {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if(curl) {
        do {
            fetchData* currentPtr = pendingFetches.front();
            currentPtr->currentStatus = fetching;

            curl_easy_setopt(curl,CURLOPT_URL,currentPtr->sUrl.c_str());
            curl_easy_setopt(curl,CURLOPT_WRITEDATA,&currentPtr->sHtml);

            CURLcode request = curl_easy_perform(curl);

            if(request != CURLE_OK) {
                currentPtr->bError = true;
                currentPtr->sError = curl_easy_strerror(request);
                currentPtr->currentStatus = error;
            } else {
                currentPtr->currentStatus = fetchSuccess;
            }

            pendingFetches.pop();

        } while (!pendingFetches.empty());

        curl_easy_cleanup(curl);

    } else {
        util::qPrint("Error initializing CURL!");
    }

    curl_global_cleanup();
    return;
}
