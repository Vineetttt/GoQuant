#include "GetPosition.h"

GetPosition::GetPosition(const std::string& access_token)
    : access_token(access_token) {}

std::string GetPosition::buildRequestUrl(const std::string& instrument_name) {
    const std::string url = "https://test.deribit.com/api/v2/private/get_position";

    // Construct the request URL with query parameters
    std::string request_url = url + "?instrument_name=" + instrument_name;

    return request_url;
}

std::string GetPosition::getPosition(const std::string& instrument_name) {
    // Build the request URL
    std::string request_url = buildRequestUrl(instrument_name);

    // Set headers
    std::vector<std::string> headers = {
        "Authorization: Bearer " + access_token,
        "Content-Type: application/json"
    };

    return http_client.get(request_url, headers);
}
