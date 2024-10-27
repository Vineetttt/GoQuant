#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <vector>
#include <curl/curl.h>

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    std::string get(const std::string& url, const std::vector<std::string>& headers = {});
    std::string post(const std::string& url, const std::string& data, const std::vector<std::string>& headers = {});

private:
    CURL* curl;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    void setHeaders(const std::vector<std::string>& headers, struct curl_slist*& curl_headers);
    std::string performRequest(const std::string& url);

    std::string response;
};

#endif