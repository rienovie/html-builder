#include "fetch.h"

std::map<std::string,fetch::fetchData> fetch::mLoadedHtmls;

fetch::fetchData& fetch::fetchUrl ( std::string sUrl ) {
    if(mLoadedHtmls.find(sUrl) != mLoadedHtmls.end()) {
        return mLoadedHtmls.at(sUrl);
    }
    mLoadedHtmls.emplace(sUrl,fetchData(sUrl));
    fetchData* dataPtr = &mLoadedHtmls.at(sUrl);

    dataPtr->currentStatus = init;

    //TODO working here, thread call to populate data

    return *dataPtr;
}
