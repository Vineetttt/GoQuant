#ifndef GET_POSITION_H
#define GET_POSITION_H

#include <string>
#include <stdexcept>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class GetPosition {
public:
    GetPosition(const std::string& access_token);
    // Method to get the positon
    std::string getPosition(const std::string& instrument_name);

private:
    std::string access_token;
    HttpClient http_client;
    std::string buildRequestUrl(const std::string& instrument_name);
};

#endif
