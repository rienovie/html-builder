#include "fetch.h"

std::map<std::string,fetch::fetchData> fetch::mLoadedHtmls;
std::queue<fetch::fetchData*> fetch::pendingFetches;

fetch::fetchData* fetch::fetchUrl ( std::string sUrl ) {
    if(mLoadedHtmls.find(sUrl) != mLoadedHtmls.end() && !mLoadedHtmls.at(sUrl).bError) {
        return &mLoadedHtmls.at(sUrl);
    }
    mLoadedHtmls.insert_or_assign(sUrl,fetchData(sUrl));
    fetchData* dataPtr = &mLoadedHtmls.at(sUrl);

    dataPtr->currentStatus = init;

    if(pendingFetches.empty()) {
        pendingFetches.push(dataPtr);
        std::thread fetchThread(threadFunc);
        fetchThread.detach();
    } else {
        pendingFetches.push(dataPtr);
    }

    return dataPtr;
}

size_t fetch::curlWriteCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    size_t dataSize = size * nmemb;
    data->append(ptr, dataSize);
    return dataSize;
}

void fetch::threadFunc () {
    util::qPrint("Boop");
    std::this_thread::sleep_for(std::chrono::seconds(1));;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if(curl) {
        do {
            fetchData* currentPtr = pendingFetches.front();
            currentPtr->currentStatus = fetching;

            curl_easy_setopt(curl,CURLOPT_URL,currentPtr->sUrl.c_str());
            curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curlWriteCallback);
            curl_easy_setopt(curl,CURLOPT_WRITEDATA,&currentPtr->sHtml);

            CURLcode request = curl_easy_perform(curl);

            if(request != CURLE_OK) {
                currentPtr->bError = true;
                currentPtr->sError = curl_easy_strerror(request);
                currentPtr->currentStatus = error;
                util::qPrint(currentPtr->sError);
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
