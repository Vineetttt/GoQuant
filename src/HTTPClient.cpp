#include "HTTPClient.h"
#include <stdexcept>

HttpClient::HttpClient() : curl(curl_easy_init()) {
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    // Set general options
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 60L); 
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);
}

HttpClient::~HttpClient() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

size_t HttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

void HttpClient::setHeaders(const std::vector<std::string>& headers, struct curl_slist*& curl_headers) {
    for (const auto& header : headers) {
        curl_headers = curl_slist_append(curl_headers, header.c_str());
    }
}

std::string HttpClient::performRequest(const std::string& url) {
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);


    // Perform the request and reset headers after each call
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }

    return response;
}

std::string HttpClient::get(const std::string& url, const std::vector<std::string>& headers) {
    curl_easy_reset(curl);  // Reset options to prepare for the next request
    response.clear();

    struct curl_slist* curl_headers = nullptr;
    setHeaders(headers, curl_headers);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);

    std::string result = performRequest(url);

    curl_slist_free_all(curl_headers);  // Clean up headers
    return result;
}

std::string HttpClient::post(const std::string& url, const std::string& data, const std::vector<std::string>& headers) {
    curl_easy_reset(curl);  // Reset options to prepare for the next request
    response.clear();

    struct curl_slist* curl_headers = nullptr;
    setHeaders(headers, curl_headers);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    std::string result = performRequest(url);

    curl_slist_free_all(curl_headers);  // Clean up headers
    return result;
}