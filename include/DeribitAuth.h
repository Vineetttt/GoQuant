#ifndef DERIBIT_AUTH_H
#define DERIBIT_AUTH_H

#include <string>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class DeribitAuth {
public:
    DeribitAuth(const std::string& clientId, const std::string& clientSecret);
    std::string authenticate();
    std::string getAccessToken() const { return access_token; }

private:
    std::string clientId;
    std::string clientSecret;
    std::string access_token;
    HttpClient http_client;
};
#endif