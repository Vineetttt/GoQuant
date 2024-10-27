#include "DeribitAuth.h"
#include <stdexcept>

DeribitAuth::DeribitAuth(const std::string& clientId, const std::string& clientSecret)
    : clientId(clientId), clientSecret(clientSecret) {}

std::string DeribitAuth::authenticate() {
    std::string url = "https://test.deribit.com/api/v2/public/auth?client_id=" + clientId + "&client_secret=" + clientSecret + "&grant_type=client_credentials";

    std::vector<std::string> headers = { "Content-Type: application/json" };
    std::string response = http_client.get(url, headers);

    auto json_response = nlohmann::json::parse(response, nullptr, false);
    if (json_response.contains("result") && json_response["result"].contains("access_token")) {
        access_token = json_response["result"]["access_token"];
    }

    return response;
}