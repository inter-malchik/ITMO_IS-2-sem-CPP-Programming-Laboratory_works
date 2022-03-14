#include "easyrequest.hpp"


EasyRequester::EasyRequester(const char *url) : url_(url) { curl_init(); }

EasyRequester::EasyRequester(std::string &url) : url_(url) { curl_init(); }

std::string& EasyRequester::perform() {
    readBuffer.clear();
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        throw std::runtime_error("error, cannot perform request");
    return readBuffer;
}


void EasyRequester::curl_init() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl error");
    curl_easy_setopt(curl, CURLOPT_URL, url_.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
}

size_t EasyRequester::WriteCallback(void *contents, size_t size, size_t nmemb, EasyRequester *userp) {

    size_t realsize = size * nmemb;
    userp->readBuffer.append((char *) contents, realsize);
    return realsize;
}

