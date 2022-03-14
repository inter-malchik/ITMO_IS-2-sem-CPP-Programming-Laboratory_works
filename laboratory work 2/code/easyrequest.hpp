#ifndef C___EASYREQUEST_HPP
#define C___EASYREQUEST_HPP
#include <string>
#include <exception>
#include <curl/curl.h>

class EasyRequester {
public:
    explicit EasyRequester(const char *url);

    explicit EasyRequester(std::string &url);

    std::string& perform();

protected:
    void curl_init();

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, EasyRequester *userp);

private:
    std::string url_;
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
};

#endif //C___EASYREQUEST_HPP
